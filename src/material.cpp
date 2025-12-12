#include <algorithm>
#include <cmath>

#include "material.hpp"
#include "util.hpp"

BasicMaterial::BasicMaterial(Color color, float refl)
    : color(color)
    , refl(refl) {
}

Color BasicMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal,
    Scene const* scene, int recursion_depth) const {
    // Make sure the normal points in opposite direction from the incoming ray
    Vector n = (normal * incoming > 0) ? -normal : normal;

    float a = scene->get_ambient() * (1 - this->refl); // ambient light
    Color l_ambient = this->color * a;
    Color color = l_ambient; // tracks the total color

    // iterate over the light sources
    for (auto&& light : scene->get_visible_point_lights(point + 1e-4 * n)) {
        // diffuse light
        Vector lt = !(light - point); // unit vector pointing to the light source
        Color l_diffuse = this->color * ((1 - a) * (1 - this->refl) * std::max(0.0f, n * lt));

        // specular light
        Vector h = !(lt - !incoming); // unit vector halfway between `incoming` and `lt`, pointing out
        Color l_specular = scene->get_specular()
            * std::pow(std::max(0.0f, h * n), scene->get_sp())
            * Color::white();

        color = color + l_diffuse + l_specular;
    }

    // reflection
    if (this->refl > 0 && recursion_depth > 0) {
        Vector reflected = incoming - 2.0f * (incoming >> n); // direction of reflected ray
        Color l_reflected = (1 - a) * this->refl * scene->trace(Ray(point + 1e-4 * n, reflected), recursion_depth - 1);
        color = color + l_reflected;
    }

    return color;
}

TransparentMaterial::TransparentMaterial(float ior)
    : ior(ior) {
}

/**
 * @brief Compute the direction of refraction.
 * @param incoming The unit incoming ray.
 * @param normal The unit normal ray pointing towards the incoming ray.
 * @param eta IOR of old medium / IOR of new medium
 * @return The unit vector in the refracted direction according to Snell's law,
 * or empty if total internal refraction occurs
 */
std::optional<Vector> refract(Vector incoming, Vector normal, float eta) {
    // cos (theta_i) where theta_i is the angle of incoming ray
    float cosi = -(incoming * normal);
    // Snell's law
    // sin^2 (theta_t) where theta_t is the angle of transmitted ray
    float sint2 = eta * eta * (1 - cosi * cosi);
    if (sint2 > 1.0f) {
        // total internal reflection
        return {};
    }
    // cos (theta_t)
    float cost = std::sqrt(1.0f - sint2);
    // A formula not obvious at all
    return eta * incoming + (eta * cosi - cost) * normal;
}

Color TransparentMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal,
    Scene const* scene, int recursion_depth) const {
    if (recursion_depth <= 0) {
        return Color::black();
    }

    float eta = 1.0f / this->ior; // IOR of incoming / IOR of transmitted
    Vector n = normal; // unit normal pointing towards the incoming ray
    if (incoming * normal > 0.0f) {
        // The ray comes from inside
        eta = this->ior;
        n = -normal;
    }

    // Compute direction and color of reflection
    Vector reflected = incoming - 2.0f * (incoming >> n);
    Color l_reflected = scene->trace(Ray(point + 1e-4 * n, reflected), recursion_depth - 1);
    // Compute direction of refraction
    std::optional<Vector> refracted = refract(!incoming, n, eta);
    // Check total internal reflection
    if (!refracted) {
        return l_reflected;
    }
    // Compute color of refraction
    Color l_refracted = scene->trace(Ray(point - 1e-4 * n, refracted.value()), recursion_depth - 1);

    // Fresnel equations for computing the ratio of light reflected
    float cosi = -(!incoming) * n;
    float cost = -(refracted.value() * n);
    // I'm not sure which one is R_s and which one is R_p, but
    // but it doesn't matter anyways
    float r_s = (eta * cosi - cost) / (eta * cosi + cost);
    float r_p = (eta * cost - cosi) / (eta * cost + cosi);
    float kr = (r_s * r_s + r_p * r_p) / 2.0f; // reflection ratio

    return kr * l_reflected + (1 - kr) * l_refracted;
}

// Utility function, it's there just because it'll be called twice later
float geometry_schlick_ggx(float cos, float k) {
    return cos / (cos * (1 - k) + k);
}

PBRMaterial::PBRMaterial(Color color, float roughness, float metallic, float reflectance)
    : color(color)
    , roughness(roughness)
    , metallic(metallic)
    , reflectance(reflectance) {
}

Color PBRMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal,
    Scene const* scene, int) const {
    // Make sure the normal points in opposite direction from the incoming ray
    Vector n = (normal * incoming > 0) ? -normal : normal;

    float a = scene->get_ambient(); // ambient light
    Color l_ambient = this->color * a;
    Color color = l_ambient; // tracks the total color

    // iterate over the light sources
    for (auto&& light : scene->get_visible_point_lights(point + 1e-4 * n)) {
        // inverse square law of point light
        // this should really be done in a `Light` class or equivalent
        float distance2 = (light - point) * (light - point);
        Color l_in = (5 / distance2) * Color::white();

        // Compute BRDF from Cook-Torrance model
        // Based on https://learnopengl.com/PBR/Theory

        // setup
        Vector lt = !(light - point); // unit vector towards the light source
        Vector v = -!incoming; // unit vector towards the incoming direction
        Vector h = !(lt + v); // unit vector halfway between `v` and `lt`
        // Why do I think all of them are guaranteed to be nonnegative?
        // I certainly won't be wrong if I enforce them anyways.
        float cos_l = n * lt;
        float cos_v = n * v;
        float cos_h = std::max(n * h, 0.0f);
        float cos_v_h = std::max(v * h, 0.0f);

        // Fresnel equation (Schlick approximation)
        float base_reflectance = 0.16f * reflectance * reflectance;
        Color f0 = (1 - metallic) * base_reflectance * Color::white() + metallic * this->color;
        Color fresnel = f0 + (Color::white() - f0) * std::pow((1 - cos_v_h), 5.0f);

        // diffuse light (lambert)
        Color f_diffuse = (1 / kPi) * this->color;

        // specular light
        // Normal distribution function (Trowbridge-Reitz)
        // distribution of normals, not the normal distribution in statistics!
        float a2 = this->roughness * this->roughness;
        float tmp1 = cos_h * cos_h * (a2 - 1) + 1;
        float ndf = a2 / (kPi * tmp1 * tmp1);
        // Geometry function
        float k = (this->roughness + 1) * (this->roughness + 1) / 8;
        float geo = geometry_schlick_ggx(cos_v, k) * geometry_schlick_ggx(cos_l, k);

        float f_specular = ndf * geo / (4 * cos_v * cos_l);

        Color brdf = (1 - metallic) * ((Color::white() - fresnel) * f_diffuse)
            + fresnel * f_specular;

        color = color + brdf * l_in * cos_l;
    }

    return color;
}

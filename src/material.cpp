#include <algorithm>
#include <cmath>
#include <utility>

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
        Vector lt = !light.get().get_direction(point); // unit vector pointing to the light source
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

// Hammersley sequence to generate uniform samples in [0, 1]^2
// https://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
std::pair<float, float> hammersley(int i, int n) {
    uint32_t bits = i;
    // reverse the bits
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1) | ((bits & 0xAAAAAAAAu) >> 1);
    bits = ((bits & 0x33333333u) << 2) | ((bits & 0xCCCCCCCCu) >> 2);
    bits = ((bits & 0x0F0F0F0Fu) << 4) | ((bits & 0xF0F0F0F0u) >> 4);
    bits = ((bits & 0x00FF00FFu) << 8) | ((bits & 0xFF00FF00u) >> 8);
    float y = (float)(bits) * 2.3283064365386963e-10; // / 0x100000000
    return { ((float)i / (float)n), y };
}

// Normal distribution function (Trowbridge-Reitz/GGX)
// distribution of normals, not the normal distribution in statistics!
float trowbridge_reitz(float a2, float cos_h) {
    float tmp1 = cos_h * cos_h * (a2 - 1) + 1;
    return a2 / (kPi * tmp1 * tmp1);
}

// Fresnel equation (Schlick approximation)
Color fresnel_schlick(Color f0, float cos_v_h) {
    return f0 + (Color::white() - f0) * std::pow((1 - cos_v_h), 5.0f);
}

// Compute BRDF from Cook-Torrance model
Color cook_torrance(Vector n, Vector lt, Vector v, Color color, Color f0, float a2, float k, float metallic) {
    // Based on https://learnopengl.com/PBR/Theory
    // setup
    Vector h = !(lt + v); // unit vector halfway between `v` and `lt`
    // Why do I think all of them are guaranteed to be nonnegative?
    // I certainly won't be wrong if I enforce them anyways.
    float cos_l = n * lt;
    float cos_v = n * v;
    float cos_h = std::max(n * h, 0.0f);
    float cos_v_h = std::max(v * h, 0.0f);

    // Fresnel equation (Schlick approximation)
    Color fresnel = fresnel_schlick(f0, cos_v_h);

    // diffuse light (lambert)
    Color f_diffuse = (1 / kPi) * color;

    // specular light
    // NDF
    float ndf = trowbridge_reitz(a2, cos_h);
    // Geometry function
    float geo = geometry_schlick_ggx(cos_v, k) * geometry_schlick_ggx(cos_l, k);

    float f_specular = ndf * geo / (4 * cos_v * cos_l);

    return (1 - metallic) * ((Color::white() - fresnel) * f_diffuse)
        + fresnel * f_specular;
}

PBRMaterial::PBRMaterial(Color color, float roughness, float metallic, float reflectance, int num_samples)
    : color(color)
    , roughness(roughness)
    , metallic(metallic)
    , reflectance(reflectance)
    , num_samples(num_samples) {
}

Color PBRMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal,
    Scene const* scene, int recursion_depth) const {
    // Make sure the normal points in opposite direction from the incoming ray
    Vector n = (normal * incoming > 0) ? -normal : normal;

    float a = scene->get_ambient(); // ambient light
    Color l_ambient = this->color * a;
    Color color = l_ambient; // tracks the total color

    // Should really be cached in the constructor
    float base_reflectance = 0.16f * this->reflectance * this->reflectance;
    Color f0 = (1 - this->metallic) * base_reflectance * Color::white() + this->metallic * this->color;
    float a2 = this->roughness * this->roughness;
    float k = (this->roughness + 1) * (this->roughness + 1) / 8;

    // iterate over the light sources
    for (auto&& light : scene->get_visible_point_lights(point + 1e-4 * n)) {
        Color l_in = light.get().get_intensity(point);

        Vector lt = !light.get().get_direction(point); // unit vector towards the light source
        Vector v = -!incoming; // unit vector towards the incoming direction

        Color brdf = cook_torrance(n, lt, v, this->color, f0, a2, k, this->metallic);

        color = color + brdf * l_in * (n * lt);
    }

    if (recursion_depth > 0) {
        // Importance sampling of specular BRDF
        // https://google.github.io/filament/Filament.md.html#annex/importancesamplingfortheibl
        for (int i = 0; i < num_samples; i++) {
            auto [u1, u2] = hammersley(i, this->num_samples);
            // Sample polar coordinate of the halfway vector wrt the `n` axis
            // Probability density function is NDF * (n * h)
            // This is probably guaranteed to be valid by some property of NDF
            // For some reason in all sources theta is called phi and vice versa
            float theta = 2 * kPi * u2;
            float cos_phi = std::sqrt((1 - u1) / (1 + (a2 - 1) * u1));
            float sin_phi = std::sqrt(1 - cos_phi * cos_phi);

            // Sample halfway vector in local cartesian coordinates
            Vector h_local(std::cos(theta) * sin_phi, std::sin(theta) * sin_phi, cos_phi);

            // Sample halfway vector in space coordinates
            // Any unit vector that is not collinear with the normal
            Vector tmp = std::abs(n.z) < 0.999 ? Vector(0.0, 0.0, 1.0) : Vector(1.0, 0.0, 0.0);
            Vector tangent = !(tmp ^ n);
            Vector bitangent = !(n ^ tangent);
            // Technically shouldn't need to normalize
            Vector h = !(tangent * h_local.x + bitangent * h_local.y + n * h_local.z);
            Vector v = -!incoming;
            Vector lt = !(incoming - 2.0f * (incoming >> h)); // direction of reflected ray

            float cos_h = cos_phi;
            float cos_l = lt * n;
            float cos_v = std::max(v * n, 0.0f);
            float cos_v_h = std::max(h * v, 0.0f);

            if (cos_l <= 0.0f) {
                continue;
            }

            // Redundant calculations are cancelled
            Color fresnel = fresnel_schlick(f0, cos_v_h);
            float geo = geometry_schlick_ggx(cos_v, k) * geometry_schlick_ggx(cos_l, k);
            Color multiplier = fresnel * (geo * cos_v_h / (cos_v * cos_h));

            // I'm too lazy to play with recursion_depth so just don't do recursion
            // Also it'd be too slow since we are doing a lot of sampling
            Color l_in = scene->trace(Ray(point + 1e-4 * n, lt), 0);

            color = color + multiplier * l_in * (1.0f / this->num_samples);
        }
    }

    return color;
}

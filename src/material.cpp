#include <algorithm>
#include <cmath>

#include "material.hpp"

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

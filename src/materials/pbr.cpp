#include <algorithm>
#include <cmath>
#include <utility>

#include "../util.hpp"
#include "pbr.hpp"

// Utility function, it's there just because it'll be called multiple times
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

    // ambient light
    Color l_ambient = this->color * scene->get_ambient();
    Color color = l_ambient; // tracks the total color

    // Should really be cached in the constructor
    float base_reflectance = 0.16f * this->reflectance * this->reflectance;
    Color f0 = (1 - this->metallic) * base_reflectance * Color::white() + this->metallic * this->color;
    float a2 = this->roughness * this->roughness;
    float k = (this->roughness + 1) * (this->roughness + 1) / 8;

    Vector v = -!incoming; // unit vector towards the incoming direction

    // iterate over the light sources
    for (auto&& light : scene->get_visible_point_lights(point + 1e-4 * n)) {
        Color l_in = light.get().get_intensity(point);
        Vector lt = !light.get().get_direction(point); // unit vector towards the light source
        Color brdf = cook_torrance(n, lt, v, this->color, f0, a2, k, this->metallic);
        color = color + brdf * l_in * (n * lt);
    }

    if (recursion_depth > 0) {
        // Importance sampling of specular reflection
        // https://google.github.io/filament/Filament.md.html#annex/importancesamplingfortheibl
        for (int i = 0; i < num_samples; i++) {
            auto [u1, u2] = hammersley(i, this->num_samples);
            // Sample polar coordinate of the halfway vector wrt the `n` axis
            // Probability density function (PDF) of h is NDF * (n * h)
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
            // direction of reflected ray
            Vector lt = !(incoming - 2.0f * (incoming >> h));

            float cos_h = cos_phi;
            float cos_l = lt * n;
            float cos_v = v * n;
            float cos_v_h = std::max(h * v, 0.0f);

            if (cos_l <= 0.0f) {
                continue;
            }

            // Calculate (specular BRDF * cosl / sampling PDF of lt)
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

#include <algorithm>
#include <cmath>

#include "material.hpp"

BasicMaterial::BasicMaterial(Color color, float refl)
    : color(color)
    , refl(refl)
{
}

Color BasicMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal,
    Scene const* scene, int recursion_depth) const
{
    // ambient light
    float a = scene->get_ambient() * (1 - this->refl);
    Color l_ambient = this->color * a;
    Color color = l_ambient; // tracks the total color

    // iterate over the light sources
    for (auto&& light : scene->get_point_lights(point - 1e-6 * incoming)) {
        // diffuse light
        Vector lt = !(light - point); // unit vector pointing to the light source
        Color l_diffuse = this->color * ((1 - a) * (1 - this->refl) * std::max(0.f, normal * lt));

        // specular light
        Vector h = !(lt - !incoming); // unit vector halfway between `incoming` and `lt`, pointing out
        Color l_specular = scene->get_specular()
            * std::pow(std::max(0.f, h * normal), scene->get_sp())
            * Color::white();

        color = color + l_diffuse + l_specular;
    }

    // reflection
    if (recursion_depth > 0) {
        Vector reflected = incoming - 2.f * (incoming >> normal); // direction of reflected ray
        Color l_reflected = scene->trace(
            Ray(point + 1e-6 * reflected, reflected), recursion_depth - 1);
        color = color + l_reflected;
    }

    return color;
}

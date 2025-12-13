#include <algorithm>
#include <cmath>

#include "basic.hpp"

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

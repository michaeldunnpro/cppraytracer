#include <algorithm>
#include <cmath>

#include "material.hpp"

Color BasicMaterial::get_color(
    Vector const& incoming, Point const& point, Vector const& normal, Scene const* scene) const
{
    double a = scene->get_ambient() * (1 - this->refl);
    Color l_ambient = this->color * a;
    Color color = l_ambient;

    for (auto&& light : scene->get_point_lights()) {
        Vector lt = !(light - point);
        Color l_diffuse = this->color * ((1 - a) * (1 - this->refl) * std::max(0.f, normal * lt));

        Vector h = !(lt - !incoming);
        Color l_specular = scene->get_specular() * std::pow(std::max(0.f, h * normal), scene->get_sp()) * Color::white();

        color = color + l_diffuse + l_specular;
    }

    Vector reflected = incoming - 2.f * incoming.project(normal);
    Color l_reflected = scene->trace(Ray(point, reflected));
    color = color + l_reflected;

    return color;
}

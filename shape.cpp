#include "shape.hpp"

template <typename T>
inline BasicSphere<T>::BasicSphere(Point center, float radius, T material)
    : center(center)
    , radius(radius)
    , material(material)
{
}

template <typename T>
std::optional<float> BasicSphere<T>::intersect_first(Ray const& ray) const
{
    float a = ray.direction * ray.direction;
    float b = 2 * ray.direction * (ray.origin - this->center);
    float c = (ray.origin - this->center) * (ray.origin - this->center) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return {};
    }
    float root = std::sqrt(discriminant);
    float t1 = (-b - root) / (2 * a);
    float t2 = (-b + root) / (2 * a);
    if (t1 > 0) {
        return t1;
    }
    if (t2 > 0) {
        return t2;
    }
    return {};
}

template <typename T>
Vector BasicSphere<T>::normal_at(Point const& point) const
{
    return point - this->center;
}

template <typename T>
std::unique_ptr<Material> BasicSphere<T>::material_at(Point const&) const
{
    return std::unique_ptr<Material>(this->material);
}

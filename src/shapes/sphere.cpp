#include "sphere.hpp"

Sphere::Sphere(Point center, float radius)
    : center(center)
    , radius(radius) {
}

std::optional<float> Sphere::intersect_first(Ray const& ray) const {
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

Vector Sphere::normal_at(Point const& point) const {
    return !(point - this->center);
}
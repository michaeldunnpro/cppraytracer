#include "ray.hpp"

Ray::Ray(Point origin, Vector direction)
    : origin(origin)
    , direction(direction) {
}

Point Ray::at(float t) const {
    return this->origin + t * this->direction;
}

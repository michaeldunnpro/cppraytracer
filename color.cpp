#include "color.hpp"

Color::Color(float const red, float const green, float const blue) : r(red), g(green), b(blue) {}

void Color::white() {
    this->r = 255.0f;
    this->g = 255.0f;
    this->b = 255.0f;
}

void Color::clamp() {
    if (this->r < 0.0f) {
        this->r = 0.0f;
    }
    if (this->r > 255.0f) {
        this->r = 255.0f;
    }
    if (this->g < 0.0f) {
        this->g = 0.0f;
    }
    if (this->g > 255.0f) {
        this->g = 255.0f;
    }
    if (this->b < 0.0f) {
        this->b = 0.0f;
    }
    if (this->b > 255.0f) {
        this->b = 255.0f;
    }
}

Color operator+(Color const& lhs, Color const& rhs) {
    // Define color addition
    return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}

Color operator-(Color const& lhs, Color const& rhs) {
    // Define color subtraction
    return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}

Color operator*(Color const& color, float scalar) {
    // Define color scaling
    return Color(color.r * scalar, color.g * scalar, color.b * scalar);
}

Color operator*(float scalar, Color const& color) {
    // Define color scaling (commutative)
    return Color(color.r * scalar, color.g * scalar, color.b * scalar);
}
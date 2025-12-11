#include "color.hpp"

Color::Color(float const red, float const green, float const blue) : 
  r(red), g(green), b(blue) {}

Color Color::black() {
    return Color(0.0f, 0.0f, 0.0f);
}

Color Color::white() {
    return Color(255.0f, 255.0f, 255.0f);
}

std::array<float, 3> Color::getRGB() const {
    // Return ownership of the array
    return { this->r, this->g, this->b };
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

// Define color addition
Color operator+(Color const& lhs, Color const& rhs) {
    return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}

// Define color subtraction
Color operator-(Color const& lhs, Color const& rhs) {
    return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}


// Define color scaling
Color operator*(Color const& color, float scalar) {
    return Color(color.r * scalar, color.g * scalar, color.b * scalar);
}

// Define color scaling (commutative)
Color operator*(float scalar, Color const& color) {
    return Color(color.r * scalar, color.g * scalar, color.b * scalar);
}

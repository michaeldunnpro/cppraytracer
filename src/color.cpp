#include <cmath>

#include "color.hpp"

/**
 * @brief Convert sRGB (color encoding in most image formats) to linear color
 */
float gamma_correction(float val, float gamma) {
    return std::pow(val / 255, gamma);
}

/**
 * @brief Convert linear color to sRGB (color encoding in most image formats)
 */
float inv_gamma_correction(float val, float gamma) {
    return std::pow(val, 1/gamma) * 255;
}

Color::Color(float const red, float const green, float const blue) : 
  r(red), g(green), b(blue) {}

Color Color::from_rgb(float r, float g, float b, float gamma) {
    return Color(gamma_correction(r, gamma), gamma_correction(g, gamma), gamma_correction(b, gamma));
}

Color Color::raw(float r, float g, float b) {
    return Color(r, g, b);
}

Color Color::black() {
    return Color(0.0f, 0.0f, 0.0f);
}

Color Color::white() {
    return Color(1.0f, 1.0f, 1.0f);
}

std::array<float, 3> Color::getRGB(float gamma) const {
    // Return ownership of the array
    return { inv_gamma_correction(this->r, gamma), inv_gamma_correction(this->g, gamma), inv_gamma_correction(this->b, gamma) };
}

void Color::clamp() {
    this->r = std::clamp(this->r, 0.0f, 1.0f);
    this->g = std::clamp(this->g, 0.0f, 1.0f);
    this->b = std::clamp(this->b, 0.0f, 1.0f);
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

Color operator*(Color const& lhs, Color const& rhs) {
    return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

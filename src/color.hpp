#pragma once

#include <array>
#include <memory>

/**
 * Color definition
 * Instantiate using Color(r, g, b) where r, g, b are floats in [0.0, 255.0]
 */
class Color {
private:
    float r;
    float g;
    float b;

public:
    // It is arguable whether arguments passed by value should be
    // marked as const either in the header or the implementaion
    // see https://stackoverflow.com/q/117293
    Color(float const r, float const g, float const b);
    static Color black(); // Create a color representing black
    static Color white(); // Create a color representing white
    void clamp(); // Clamp color values to valid range (in-place)
    std::array<float, 3> getRGB() const; // Get array to RGB values
    friend Color operator+(Color const&, Color const&); // Color addition
    friend Color operator-(Color const&, Color const&); // Color subtraction
    friend Color operator*(Color const&, float); // Color scaling
    friend Color operator*(float, Color const&); // Color scaling (commutative)
};

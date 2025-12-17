#pragma once

#include <array>
#include <memory>

/**
 * Color definition
 * Instantiate using Color::from_rgb(r, g, b) where r, g, b are floats in [0.0, 255.0]
 */
class Color {
private:
    float r;
    float g;
    float b;
    // Raw constructor, used internally
    // We want to encourage users to use gamma correction/sRGB,
    // so the raw constructor is hidden as private. If users want they may
    // use Color::raw() instead.
    // It is arguable whether arguments passed by value should be
    // marked as const either in the header or the implementaion
    // see https://stackoverflow.com/q/117293
    Color(float const r, float const g, float const b);

public:
    // Create a color from the given rgb in the usual [0, 255] range
    // Gamma correction is applied, so input is approximarely sRGB
    // Color::from_rgb(r, g, b).get_rgb() is guaranteed to give original r, g, b
    static Color from_rgb(float r, float g, float b, float gamma = 2.2f);
    // Create a color using the internal Color representation
    // where (1.0f, 1.0f, 1.0f) is white
    static Color raw(float r, float g, float b);
    // Create a color representing black (additive identity)
    static Color black();
    // Create a color representing white (multiplicative identity)
    static Color white();
    void clamp(); // Clamp color values to valid range (in-place)
    std::array<float, 3> get_rgb(float gamma = 2.2f) const; // Get array to RGB values
    friend Color operator+(Color const&, Color const&); // Color addition
    friend Color operator-(Color const&, Color const&); // Color subtraction
    friend Color operator*(Color const&, float); // Color scaling
    friend Color operator*(float, Color const&); // Color scaling (commutative)
    // Pointwise multiplication (filter)
    friend Color operator*(Color const&, Color const&);
};

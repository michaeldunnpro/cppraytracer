#pragma once
#include <memory>
#include <stdlib.h>

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
    Color(float const r, float const g, float const b);
    static Color white(); // Create a color representing white
    void clamp(); // Clamp color values to valid range (in-place)
    std::unique_ptr<float[]> getRGB(); // Get pointer to RGB values
    friend Color operator+(Color const&, Color const&); // Color addition
    friend Color operator-(Color const&, Color const&); // Color subtraction
    friend Color operator*(Color const&, float); // Color scaling
    friend Color operator*(float, Color const&); // Color scaling (commutative)
};

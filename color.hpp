#pragma once

// Color definition
// Instantiate using Color(r, g, b) where r, g, b are floats in [0.0, 255.0]
class Color {
    public:
        Color(float const r, float const g, float const b);
        void white(); // Convert color to white
        void clamp(); // Clamp color values to valid range
        float* getRGB(); // Get pointer to RGB values
        friend Color operator+(Color const&, Color const&); // Color addition
        friend Color operator-(Color const&, Color const&); // Color subtraction
        friend Color operator*(Color const&, float); // Color scaling
        friend Color operator*(float, Color const&); // Color scaling (commutative)
    private:
        float r;
        float g;
        float b;
};

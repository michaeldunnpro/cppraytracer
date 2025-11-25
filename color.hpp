#pragma once

// Color definition
class Color {
    public:
        void white(); // Set color to white
        void clamp(); // Clamp color values to valid range
        friend Color operator+(Color const&, Color const&); // Color addition
        friend Color operator-(Color const&, Color const&); // Color subtraction
        friend Color operator*(Color const&, float); // Color scaling
        friend Color operator*(float, Color const&); // Color scaling (commutative)
    private:
        Color(float const r, float const g, float const b);
        float r;
        float g;
        float b;
};

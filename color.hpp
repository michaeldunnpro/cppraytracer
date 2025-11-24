#pragma once

class Color {
public:
    static Color white();
    friend Color operator+(Color const&, Color const&);
    friend Color operator*(Color const&, float);
    friend Color operator*(float, Color const&);
};

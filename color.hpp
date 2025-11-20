#pragma once

class Color {
public:
    friend Color operator+(Color const&, Color const&);
    friend Color operator*(Color const&, double const&);
    friend Color operator*(double const&, Color const&);
};

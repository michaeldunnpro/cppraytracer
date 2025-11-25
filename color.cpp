#include "color.hpp"

Color::Color(float const red, float const green, float const blue) : r(red), g(green), b(blue) {}

 void Color::white() {
    this->r = 255.0f;
    this->g = 255.0f;
    this->b = 255.0f;
}

float* Color::getRGB() {
    float* rgb = new float[3];
    rgb[0] = this->r;
    rgb[1] = this->g;
    rgb[2] = this->b;
    return rgb;
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

#ifdef debug_color_cpp
#include <cassert>
#include <iostream>
int main(){
    Color c1(100.0f, 150.0f, 200.0f);
    Color c2(50.0f, 75.0f, 100.0f);
    
    Color c3 = c1 + c2;
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f && c3.getRGB()[2] == 300.0f);
    
    Color c4 = c1 - c2;
    assert(c4.getRGB()[0] == 50.0f && c4.getRGB()[1] == 75.0f && c4.getRGB()[2] == 100.0f);
    
    Color c5 = c1 * 2.0f;
    assert(c5.getRGB()[0] == 200.0f && c5.getRGB()[1] == 300.0f && c5.getRGB()[2] == 400.0f);
    
    Color c6 = 0.5f * c2;
    assert(c6.getRGB()[0] == 25.0f && c6.getRGB()[1] == 37.5f && c6.getRGB()[2] == 50.0f);
    
    c3.clamp();
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f && c3.getRGB()[2] == 255.0f); // b should be clamped to 255
    
    c3.white();
    assert(c3.getRGB()[0] == 255.0f && c3.getRGB()[1] == 255.0f && c3.getRGB()[2] == 255.0f); // c3 should be white now

    std::cout << "Color class compiled successfully." << std::endl;
    return 0;

}

#endif
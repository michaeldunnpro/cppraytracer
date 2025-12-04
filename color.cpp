#include "color.hpp"

Color::Color(float const red, float const green, float const blue) : 
  r(red), g(green), b(blue) {}

Color Color::white() {
    return Color(255.0f, 255.0f, 255.0f);
}

std::unique_ptr<float[]> Color::getRGB() {
    std::unique_ptr<float[]> rgb = std::make_unique<float[]>(3);
    rgb[0] = this->r;
    rgb[1] = this->g;
    rgb[2] = this->b;
    return rgb; // Return ownership of the array
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

#include <cassert>
#include <iostream>

#ifdef debug_color_cpp

int main(){
    Color c1(100.0f, 150.0f, 200.0f);
    Color c2(50.0f, 75.0f, 100.0f);
    
    Color c3 = c1 + c2;
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f && 
        c3.getRGB()[2] == 300.0f);
    
    Color c4 = c1 - c2;
    assert(c4.getRGB()[0] == 50.0f && c4.getRGB()[1] == 75.0f && 
        c4.getRGB()[2] == 100.0f);
    
    Color c5 = c1 * 2.0f;
    assert(c5.getRGB()[0] == 200.0f && c5.getRGB()[1] == 300.0f && 
        c5.getRGB()[2] == 400.0f);
    
    Color c6 = 0.5f * c2;
    assert(c6.getRGB()[0] == 25.0f && c6.getRGB()[1] == 37.5f && 
        c6.getRGB()[2] == 50.0f);
    
    c3.clamp();
    assert(c3.getRGB()[0] == 150.0f && c3.getRGB()[1] == 225.0f && 
        c3.getRGB()[2] == 255.0f); // b should be clamped to 255


    std::cout << "Color class compiled successfully." << std::endl;
    return 0;

}

#endif

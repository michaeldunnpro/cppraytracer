#pragma once


class Point { // Point definition
    public:
        Point(float const x, float const y, float const z);
        float const x;
        float const y; 
        float const z;
};

class Vector { // Vector definition
    public:
        Vector(float const x, float const y, float const z);
        float const x;
        float const y;
        float const z;
};

// Vector Operations 

Vector operator+(const Vector& lhs, const Vector& rhs); // Vector addition
Vector operator-(const Vector& lhs, const Vector& rhs); // Vector subtraction
Vector operator^(const Vector& lhs, const Vector& rhs); // Cross product
float operator*(const Vector& lhs, const Vector& rhs);   // Dot product
float operator~(const Vector& vec);                       // Magnitude
Vector operator-(const Vector& vec);                      // Negation
Vector operator!(const Vector& vec);                      // Normalization
bool operator==(const Vector& lhs, const Vector& rhs);    // Equality

// Scalar actions on Vectors
Vector operator*(const Vector& vec, float scalar); // Scalar right-multiplication
Vector operator*(float scalar, const Vector& vec); // Scalar left-multiplication
Vector operator/(const Vector& vec, float scalar);  // Scalar division

// Projection operators
Vector operator>>(const Vector& vec, const Vector& onto); // Projection of vec onto
Vector operator<<(const Vector& onto, const Vector& vec); // Left-projection of vec onto

// Point Operations
Point operator+(const Point& point, const Vector& vec); // Point translation by vector
Point operator+(const Vector& vec, const Point& point);
Point operator-(const Point& point, const Vector& vec); // Point translation by negative vector
Vector operator-(const Point& lhs, const Point& rhs); // Vector from point difference
bool operator==(const Point& lhs, const Point& rhs);    // Equality


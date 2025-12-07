#pragma once

class Point { // Point definition
    public:
        Point(float const x, float const y, float const z);
        float x;
        float y; 
        float z;
};

class Vector { // Vector definition
    public:
        Vector(float const x, float const y, float const z);
        float x;
        float y;
        float z;
        Vector Rotate(Vector axis, float angle); // Intrinsic rotation of the vector
};

// Vector Operations 
Vector operator+(Vector const& lhs, Vector const& rhs); // Vector addition
Vector operator-(Vector const& lhs, Vector const& rhs); // Vector subtraction
Vector operator^(Vector const& lhs, Vector const& rhs); // Cross product
float operator*(Vector const& lhs, Vector const& rhs);  // Dot product
float operator~(Vector const& vec);                     // Magnitude
Vector operator-(Vector const& vec);                    // Negation
Vector operator!(Vector const& vec);                    // Normalization
bool operator==(Vector const& lhs, Vector const& rhs);  // Equality

// Scalar actions on Vectors
Vector operator*(Vector const& vec, float scalar); // Scalar right-multiplication
Vector operator*(float scalar, Vector const& vec); // Scalar left-multiplication
Vector operator/(Vector const& vec, float scalar); // Scalar division

// Projection operators
Vector operator>>(Vector const& vec, Vector const& onto); // Projection of vec onto
Vector operator<<(Vector const& onto, Vector const& vec); // Left-projection of vec onto

// Point Operations
Point operator+(Point const& point, Vector const& vec); // Point translation by vector
Point operator+(Vector const& vec, Point const& point);
Point operator-(Point const& point, Vector const& vec); // Point translation by negative vector
Vector operator-(Point const& lhs, Point const& rhs);   // Vector from point difference
bool operator==(Point const& lhs, Point const& rhs);    // Equality


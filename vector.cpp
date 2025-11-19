#include <cmath>
#include "vector.hpp"


// Overloads for Vector and Point classes
/*
Vector Operations:

-> Maps to vector <-
- Addition: Vector + Vector
- Subtraction: Vector - Vector
- Scalar Multiplication: Vector * float, float * Vector
- Scalar Division: Vector / float
- Cross Product: Vector ^ Vector

-> Maps to float <-
- Dot Product: Vector * Vector
- Magnitude: ~Vector

-> Unary Operations <-
- Negation: -Vector
- Normalization: !Vector

-> Equivalence Relation <-
- Equality: Vector == Vector

Point Operations:

-> Maps to Point <-
- Point Translation: Point + Vector, Point - Vector

-> Equivalence Relation <-
- Equality: Point == Point
*/



// Vector Class Implementation
Vector::Vector(float const xs, float const ys, float const zs) 
: x(xs), y(ys), z(zs) {
    // Constructor body (empty)
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
    // Define vector addition
    return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
    // Define vector subtraction
    return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}


// Actions on Vectors
// Note multiple overloads to simulate commutativity where applicable

Vector operator*(const Vector& vec, float scalar) {
    // Define scalar right-multiplication
    return Vector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector operator*(float scalar, const Vector& vec) {
    // Define scalar left-multiplication (commutative)
    return Vector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector operator/(const Vector& vec, float scalar) {
    // Define scalar division
    return Vector(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

Vector operator^(const Vector& lhs, const Vector& rhs) {
    // Define cross product
    return Vector(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}


float operator*(const Vector& lhs, const Vector& rhs) {
    // Define dot product
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float operator~(const Vector& vec) {
    // Calculate magnitude of the vector
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

// Unary Operations

Vector operator-(const Vector& vec) {
    // Define negation of the vector
    return Vector(-vec.x, -vec.y, -vec.z);
}

Vector operator!(const Vector& vec) {
    // Define normalization of the vector
    float mag = ~vec;
    return Vector(vec.x / mag, vec.y / mag, vec.z / mag);
}

bool operator==(const Vector& lhs, const Vector& rhs) {
    // Define equality check between two vectors
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}


// Point Class Implementation
Point::Point(float const xs, float const ys, float const zs) 
: x(xs), y(ys), z(zs) {
    // Constructor body (empty)
}

Point operator+(const Point& point, const Vector& vec) {
    // Define point translation by a vector
    return Point(point.x + vec.x, point.y + vec.y, point.z + vec.z);
}

Point operator-(const Point& point, const Vector& vec) {
    // Define point translation by negative vector
    return Point(point.x - vec.x, point.y - vec.y, point.z - vec.z);
}

bool operator==(const Point& lhs, const Point& rhs) {
    // Define equality check between two points
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

#ifdef debug_vector_cpp
#include <cassert>
#include <iostream>
int main() {
    // Simple test cases can be added here if needed
    Vector v1 = Vector(1.0f, 2.0f, 3.0f);
    Vector v2 = Vector(4.0f, 5.0f, 6.0f);
    Vector v3 = v1 + v2;
    assert(v3 == Vector(5.0f, 7.0f, 9.0f)); 
    Vector v4 = v2 - v1;
    assert(v4 == Vector(3.0f, 3.0f, 3.0f));
    Vector v5 = v1 * 2.0f;
    assert(v5 == Vector(2.0f, 4.0f, 6.0f));
    Vector v6 = 2.0f * v1;
    assert(v6 == Vector(2.0f, 4.0f, 6.0f));
    Vector v7 = v2 / 2.0f;
    assert(v7 == Vector(2.0f, 2.5f, 3.0f));
    Vector v8 = v1 ^ v2;
    assert(v8 == Vector(-3.0f, 6.0f, -3.0f));
    float dot = v1 * v2;
    assert(dot == 32.0f);
    float mag = ~v1;
    assert(std::abs(mag - 3.74166f) < 0.0001f);
    Vector v9 = -v1;
    assert(v9 == Vector(-1.0f, -2.0f, -3.0f));
    Vector v10 = !v1;
    mag = ~v10;
    assert(std::abs(mag - 1.0f) < 0.0001f);
    std::cout << "Vector and Point classes compiled successfully." << std::endl;
    return 0;
}
#endif
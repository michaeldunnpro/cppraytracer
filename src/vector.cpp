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

Vector Vector::rotate(float gamma, float beta, float alpha) const {
    // Intrinsic Euler: Rx(alpha) then Ry(beta) then Rz(gamma)
    // gamma about Z, beta about Y, alpha about X
    float c1 = std::cos(alpha), s1 = std::sin(alpha);
    float c2 = std::cos(beta),  s2 = std::sin(beta);
    float c3 = std::cos(gamma), s3 = std::sin(gamma);

    float new_x = x * (c2 * c3) + y * (c3 * s2 * s1 - s3 * c1) + z * (c3 * s2 * c1 + s3 * s1);
    float new_y = x * (c2 * s3) + y * (s3 * s2 * s1 + c3 * c1) + z * (s3 * s2 * c1 - c3 * s1);
    float new_z = x * (-s2)    + y * (c2 * s1)                  + z * (c2 * c1);
    return Vector(new_x, new_y, new_z);
}
    
Vector operator+(Vector const& lhs, Vector const& rhs) {
    // Define vector addition
    return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector operator-(Vector const& lhs, Vector const& rhs) {
    // Define vector subtraction
    return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}


/*
 * Actions on Vectors
 * Note multiple overloads to simulate commutativity where applicable
 */

// Define scalar right-multiplication
Vector operator*(Vector const& vec, float scalar) {
    return Vector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

// Define scalar left-multiplication (commutative)
Vector operator*(float scalar, Vector const& vec) {
    return Vector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

// Define scalar division
Vector operator/(Vector const& vec, float scalar) {
    return Vector(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

// Define cross product
Vector operator^(Vector const& lhs, Vector const& rhs) {
    return Vector(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

// Define dot product
float operator*(Vector const& lhs, Vector const& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// Calculate magnitude of the vector
float operator~(Vector const& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

/*
 * Unary Operations
 */

// Define negation of the vector
Vector operator-(Vector const& vec) {
    return Vector(-vec.x, -vec.y, -vec.z);
}

// Define normalization of the vector
Vector operator!(Vector const& vec) {
    float mag = ~vec;
    return Vector(vec.x / mag, vec.y / mag, vec.z / mag);
}

// Define equality check between two vectors
bool operator==(Vector const& lhs, Vector const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

// Vector projection operator
Vector operator>>(Vector const& vec, Vector const& onto) {
    // Define projection of vec onto
    float onto_mag_sq = onto * onto;
    if (onto_mag_sq == 0.0f) {
        return Vector(0.0f, 0.0f, 0.0f); // Avoid division by zero
    }
    float scalar = (vec * onto) / onto_mag_sq;
    return scalar * onto;
}

Vector operator<<(Vector const& onto, Vector const& vec) {
    // Define projection of vec onto onto (left-projection)
    Vector projection = vec >> onto;
    return projection;
}


// Point Class Implementation
Point::Point(float const xs, float const ys, float const zs) 
  : x(xs), y(ys), z(zs) {
    // Constructor body (empty)
}

// Define point translation by a vector
Point operator+(Point const& point, Vector const& vec) {
    return Point(point.x + vec.x, point.y + vec.y, point.z + vec.z);
}

// Define point translation by a vector (commutative)
Point operator+(Vector const& vec, Point const& point) {
    return Point(point.x + vec.x, point.y + vec.y, point.z + vec.z);
}

// Define point translation by negative vector
Point operator-(Point const& point, Vector const& vec) {
    return Point(point.x - vec.x, point.y - vec.y, point.z - vec.z);
}

// Define equality check between two points
bool operator==(Point const& lhs, Point const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

/*
 * Create vectors from point difference
 */

// Define vector from point difference
Vector operator-(Point const& lhs, Point const& rhs) {
    return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

/*
 * tests
 */

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
    // Test mutability of Point operations
    Point p1 = Point(1.0f, 2.0f, 3.0f);
    p1 = p1 + v1;
    assert(p1 == Point(2.0f, 4.0f, 6.0f));
    v1 = Vector(1.0f, 2.0f, 3.0f);
    assert(v1 == Vector(1.0f, 2.0f, 3.0f));
    std::cout << "Vector and Point classes compiled successfully." << std::endl;
    return 0;
}
#endif

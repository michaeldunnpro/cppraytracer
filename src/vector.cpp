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

Vector Vector::Rotate(Vector axis, float angle) {
    Vector v = *this;
    // Rodriguez rotation formula
    // Rodrigues' rotation formula about a unit axis
    Vector k = !axis;
    float c = std::cos(angle);
    float s = std::sin(angle);
    return v * c + (k ^ v) * s + k * ((k * v) * (1.0f - c));
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
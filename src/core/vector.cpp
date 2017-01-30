#include <core/vector.h>
#include <core/common.h>

namespace ishi {

/// Initialize all components set to 0
Vector::Vector()
    : x(0), y(0), z(0) {}

/// Initialize x, y components to passed-in values, z to 0
Vector::Vector(float xx, float yy)
    : x(xx), y(yy), z(0) {}

/// Initialize x, y, z components to passed-in values
Vector::Vector(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz) {}

/// Multiply with another vector components wise and initialize a new Vector
/// with the result.
Vector Vector::operator*(float f) const {
  return Vector(x * f, y * f, z * f);
}

/// Multiply
Vector& Vector::operator*=(float f) {
  x *= f;
  y *= f;
  z *= f;
  return *this;
}

Vector operator*(float f, const Vector& v) {
  return Vector(v.x * f, v.y * f, v.z * f);
}

Vector Vector::operator+(const Vector& v) const {
  return Vector(x + v.x, y + v.y, z + v.z);
}

Vector& Vector::operator+=(const Vector& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector Vector::operator-(const Vector& v) const {
  return Vector(x - v.x, y - v.y, z - v.z);
}

Vector& Vector::operator-=(const Vector& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector Vector::operator-() const {
  return Vector(-x, -y, -z);
}

bool Vector::operator==(const Vector& v) const {
  return ((x == v.x) && (y == v.y) && (z == v.z));
}

bool Vector::operator!=(const Vector& v) const {
  return ((x != v.x) || (y != v.y) || (z != v.z));
}

float Vector::operator[](int i) const {
  Assert(i >= 0);
  Assert(i < 3);
  return (&x)[i];
}

float Dot(const Vector &v1, const Vector &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector Cross(const Vector &v1, const Vector &v2) {
  return Vector(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

float LengthSquared(const Vector& v) {
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Length(const Vector& v) {
  return sqrt(LengthSquared(v));
}

Vector Normalize(const Vector& v) {
  float l = Length(v);
  if (l) {
    float f = 1/l;
    return Vector(v.x * f, v.y * f, v.z * f);
  } else {
    return Vector();
  }
}

/**
 * Algorithm:
 * - Construct the first output vector:
 *   + zero a one component of the input vector (must not be the only
 *     non-zero component)
 *   + swap the remaining components of the input vector; negate one of them
 * - Construct the second output vector:
 *   + taking the cross product of the input vector with the first output
 *     (the order depends on how the other two vectors are constructed)
 */
void CoordinateSystem(const Vector& v1, Vector* v2, Vector* v3) {
  float invLen;
  if (std::abs(v1.x) > std::abs(v1.y)) {
    invLen = 1.f / std::sqrt(v1.x * v1.x + v1.z * v1.z);  // normalize v2
    *v2 = Vector(v1.z * invLen, 0.f, -v1.x * invLen);
  } else {
    invLen = 1.f / std::sqrt(v1.y * v1.y + v1.z * v1.z);  // normalize v2
    *v2 = Vector(0.f, -v1.z * invLen, v1.y * invLen);
  }
  *v3 = Cross(v1, *v2);
}

}  // namespace ishi

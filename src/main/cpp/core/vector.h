#ifndef CORE_VECTOR_H_
#define CORE_VECTOR_H_

#include <iostream>

namespace ishi {

class Point;

class Vector {
public:
  /// Vector components
  float x, y, z;

public:
  /// Initialize zero vector
  Vector();

  /// Initialize vector on the x-y plane
  Vector(float xx, float yy);

  /// Initialize a full 3D vector
  Vector(float xx, float yy, float zz);

  /// Multiply with scalar and return new Vector
  Vector operator*(float f) const;

  /// Multiply with scalar and save to this Vector
  Vector &operator*=(float f);

  /// Add to another Vector and return a new Vector
  Vector operator+(const Vector &v) const;

  /// Add to another Vector and save to this Vector
  Vector &operator+=(const Vector &v);

  /// Subtract another Vector and return a new Vector
  Vector operator-(const Vector &v) const;

  /// Subtract another Vector and save to this Vector
  Vector &operator-=(const Vector &v);

  /// Return another Vector that is the additive inverse of this vector
  Vector operator-() const;

  /// Return true if two Vectors are identical
  bool operator==(const Vector &v) const;

  /// Return true if two Vectors are not identical (in direction or magnitude)
  bool operator!=(const Vector &v) const;

  /// Get Vector components by integer index
  float operator[](int i) const;

  /// Overload stream output operator
  friend std::ostream& operator<< (std::ostream &out, const Vector &point);
};

/// Multiply with a scalar and return new Vector (when scalar comes first)
Vector operator*(float f, const Vector &v);

/// Compute the dot product of two vectors
float Dot(const Vector &v1, const Vector &v2);

/// Compute the cross product of two vectors
Vector Cross(const Vector &v1, const Vector &v2);

/// Return the square of the length of the vector
float LengthSquared(const Vector &v);

/// Return the length of the vector
float Length(const Vector &v);

/// Return a new vector which is this vector normalized
Vector Normalize(const Vector &v);

/// Construct a coordinate system vector given a vector.
/// Two output vectors will be modified such that the input vector and the
/// output vectors form an orthonormal basis.
///
/// @note The function assumes that the input vector is already normalized.
void CoordinateSystem(const Vector &v1, Vector *v2, Vector *v3);

}  // namespace ishi

#endif

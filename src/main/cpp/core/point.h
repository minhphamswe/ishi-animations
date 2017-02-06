#ifndef CORE_POINT_H_
#define CORE_POINT_H_

namespace ishi {

class Vector;

class Point {
 public:
  float x, y, z;

 public:
  /// Initialize point at origin
  Point();

  /// Initialize point on the x-y plane
  Point(float xx, float yy);

  /// Initialize point in 3 dimensions
  Point(float xx, float yy, float zz);

  /// Add a vector to this point, return a new point
  Point operator+(const Vector& v) const;

  /// Add a vector to this point, return reference to this point
  Point& operator+=(const Vector& v);

  /// Subtract a vector from this point, return a new point
  Point operator-(const Vector& v) const;

  /// Subtract a vector from this point, return reference to this point
  Point& operator-=(const Vector& v);

  /// Subtract a point from this point, return a new vector
  Vector operator-(const Point& p) const;

  /// Return true if two points occupy the same position
  bool operator==(const Point& p) const;

  /// Return result by weighing points by a scalar
  Point operator*(float f) const;

  /// Add a point to this point (used to compute weighted sum of points)
  Point operator+(const Point &p) const;

  /// Divide a point by a scalar (used to compute weighted sum of points)
  Point operator/(float f) const;

  float& operator[](int i);

  /// Get value of point components by integer index (no editing)
  float operator[](int i) const;
};

/// Return result by weighing points by a scalar
Point operator*(float f, const Point &p);

/// Return the distance between two points
float Distance(const Point &p1, const Point &p2);

}  // namespace ishi

#endif

#ifndef CORE_TRANSFORM_H_
#define CORE_TRANSFORM_H_

#include <core/matrix.h>

namespace ishi {

class Point;
class Vector;
class BBox;

class Transform {
 private:
  Matrix4x4 m, mInv;

 public:
  /// Initialize an identity transform
  Transform();

  /// Initialize a transform with a matrix, calculating its inverse
  explicit Transform(const Matrix4x4 &mat);

  /// Initialize a transform with a matrix and its inverse
  Transform(const Matrix4x4 &mat, const Matrix4x4 &matInv);

  /// Transform a point and return a new point
  Point operator()(const Point &p) const;

  /// Transform a vector and return a new vector
  Vector operator()(const Vector &v) const;

  /// Transform a bounding box and return a new bounding box
  BBox operator()(const BBox &bb) const;

  /// Transform a point (in-place)
  Point& Apply(Point *p) const;

  /// Transform a vector (in-place)
  Vector& Apply(Vector *v) const;

  /// Invert the transform made to a point and return a new point
  Point Invert(const Point &p) const;

  /// Invert the transform made to a vector and return a new vector
  Vector Invert(const Vector &v) const;

  /// Invert the transform made to a point (in-place)
  Point& ApplyInvert(Point *p) const;

  /// Invert the transform made to a vector (in-place)
  Vector& ApplyInvert(Vector *v) const;

  /// Concatentate this and another transform, returning a new transform
  Transform operator*(const Transform &t2) const;

  /// Concatentate this and another transform (in-place)
  Transform& operator*=(const Transform& t2);

  /// Return the transformation matrix as a 4x4 Matrix
  Matrix4x4 Matrix() const;

  friend Transform Inverse(const Transform &t);
};

/// Return the inverse a transform as a new transform
Transform Inverse(const Transform &t);

/// Return a transform representing a translation by a vector
Transform Translate(const Vector &delta);

/// Return a transform representing a rotation around the X axis.
/// The angle of rotation should be in radian.
Transform RotateX(float angle);

/// Return a transform representing a rotation around the Y axis.
/// The angle of rotation should be in radian.
Transform RotateY(float angle);

/// Return a transform representing a rotation around the Z axis.
/// The angle of rotation should be in radian.
Transform RotateZ(float angle);

/// Return a transform representing a rotation around an arbitrary axis.
/// The angle of rotation should be in radian.
Transform Rotate(float angle, const Vector &axis);

/// Return a transform representing a general scaling in each of the axes
Transform Scale(float x, float y, float z);

/// Return a transform representing a rotation resulting in a vector/point
/// being colinear to the X axis
Transform AlignX(const Vector &v);

/// Return a transform representing a rotation resulting in a vector/point
/// being colinear to the Y axis
Transform AlignY(const Vector &v);

/// Return a transform representing a rotation resulting in a vector/point
/// being colinear to the Z axis
Transform AlignZ(const Vector &v);

}  // namespace ishi

#endif

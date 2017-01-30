#include <core/transform.h>

#include <core/common.h>
#include <core/matrix.h>

#include <core/point.h>
#include <core/vector.h>
#include <core/bbox.h>

namespace ishi {

Transform::Transform()
    : m(Matrix4x4()), mInv(Matrix4x4()) {}

Transform::Transform(const Matrix4x4& mat)
    : m(mat), mInv(Inverse(mat)) {}

Transform::Transform(const Matrix4x4& mat, const Matrix4x4& matInv)
    : m(mat), mInv(matInv) {}

Point Transform::operator()(const Point& p) const {
  float x = p.x, y = p.y, z = p.z;

  // Implicitly convert point to homogeneous coordinates
  float xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
  float yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
  float zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
  float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];

  // Implicitly convert point back to non-homogeneous coordinates
  if (wp == 1.f)
    return Point(xp, yp, zp);
  else
    return Point(xp, yp, zp) * (1/wp);
}

Vector Transform::operator()(const Vector& v) const {
  float x = v.x, y = v.y, z = v.z;
  return Vector(m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
                m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
                m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
}

BBox Transform::operator() (const BBox& bb) const {
  Point pMin = (*this)(bb.pMin);
  Point pMax = (*this)(bb.pMax);
  return BBox(pMin, pMax);
}


Point& Transform::Apply(Point *p) const {
  float x = p->x, y = p->y, z = p->z;

  // Implicitly convert point to homogeneous coordinates
  float xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
  float yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
  float zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
  float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];

  // Implicitly convert point back to non-homogeneous coordinates
  if (wp == 1.f) {
    p->x = xp;
    p->y = yp;
    p->z = zp;
  } else {
    float f = 1/wp;
    p->x = xp * f;
    p->y = yp * f;
    p->z = zp * f;
  }

  return *p;
}

Vector& Transform::Apply(Vector *v) const {
  float x = v->x, y = v->y, z = v->z;
  v->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
  v->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
  v->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
  return *v;
}

Transform Transform::operator*(const Transform& t2) const {
  Matrix4x4 mat = Mul(m, t2.m);
  Matrix4x4 matInv = Mul(t2.mInv, mInv);
  return Transform(mat, matInv);
}

Transform& Transform::operator*=(const ishi::Transform& t2) {
  m = Mul(m, t2.m);
  mInv = Mul(t2.mInv, mInv);
  return *this;
}

Point Transform::Invert(const Point& p) const {
  float x = p.x, y = p.y, z = p.z;

  // Implicitly convert point to homogeneous coordinates
  float xp = mInv.m[0][0]*x + mInv.m[0][1]*y + mInv.m[0][2]*z + mInv.m[0][3];
  float yp = mInv.m[1][0]*x + mInv.m[1][1]*y + mInv.m[1][2]*z + mInv.m[1][3];
  float zp = mInv.m[2][0]*x + mInv.m[2][1]*y + mInv.m[2][2]*z + mInv.m[2][3];
  float wp = mInv.m[3][0]*x + mInv.m[3][1]*y + mInv.m[3][2]*z + mInv.m[3][3];

  // Implicitly convert point back to non-homogeneous coordinates
  if (wp == 1.f)
    return Point(xp, yp, zp);
  else
    return Point(xp, yp, zp) * (1/wp);
}

Vector Transform::Invert(const Vector& v) const {
  float x = v.x, y = v.y, z = v.z;
  return Vector(mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z,
                mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z,
                mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z);
}

Point& Transform::ApplyInvert(Point* p) const {
  float x = p->x, y = p->y, z = p->z;

  // Implicitly convert point to homogeneous coordinates
  float xp = mInv.m[0][0]*x + mInv.m[0][1]*y + mInv.m[0][2]*z + mInv.m[0][3];
  float yp = mInv.m[1][0]*x + mInv.m[1][1]*y + mInv.m[1][2]*z + mInv.m[1][3];
  float zp = mInv.m[2][0]*x + mInv.m[2][1]*y + mInv.m[2][2]*z + mInv.m[2][3];
  float wp = mInv.m[3][0]*x + mInv.m[3][1]*y + mInv.m[3][2]*z + mInv.m[3][3];

  // Implicitly convert point back to non-homogeneous coordinates
  if (wp == 1.f) {
    p->x = xp;
    p->y = yp;
    p->z = zp;
  } else {
    float f = 1/wp;
    p->x = xp * f;
    p->y = yp * f;
    p->z = zp * f;
  }

  return *p;
}

Vector& Transform::ApplyInvert(Vector* v) const {
  float x = v->x, y = v->y, z = v->z;
  v->x = mInv.m[0][0] * x + mInv.m[0][1] * y + mInv.m[0][2] * z;
  v->y = mInv.m[1][0] * x + mInv.m[1][1] * y + mInv.m[1][2] * z;
  v->z = mInv.m[2][0] * x + mInv.m[2][1] * y + mInv.m[2][2] * z;
  return *v;
}

Matrix4x4 Transform::Matrix() const {
  return m;
}

Transform Inverse(const Transform& t) {
  return Transform(t.mInv, t.m);
}

Transform Translate(const Vector& delta) {
  Matrix4x4 mat = Matrix4x4(1.f, 0.f, 0.f, delta.x,
                            0.f, 1.f, 0.f, delta.y,
                            0.f, 0.f, 1.f, delta.z,
                            0.f, 0.f, 0.f, 1.0f);
  Matrix4x4 matInv = Matrix4x4(1.f, 0.f, 0.f, -delta.x,
                               0.f, 1.f, 0.f, -delta.y,
                               0.f, 0.f, 1.f, -delta.z,
                               0.f, 0.f, 0.f, 1.0f);
  return Transform(mat, matInv);
}

Transform RotateX(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  Matrix4x4 mat = Matrix4x4(1.f, 0.f, 0.f, 0.f,
                            0.f, c, -s, 0.f,
                            0.f, s, c, 0.f,
                            0.f, 0.f, 0.f, 1.f);
  Matrix4x4 matInv = Matrix4x4(1.f, 0.f, 0.f, 0.f,
                               0.f,   c,   s, 0.f,
                               0.f,  -s,   c, 0.f,
                               0.f, 0.f, 0.f, 1.f);
  return Transform(mat, matInv);
}

Transform RotateY(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  Matrix4x4 mat = Matrix4x4(c, 0.f, s, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            -s, 0.f, c, 0.f,
                            0.f, 0.f, 0.f, 1.f);
  Matrix4x4 matInv = Matrix4x4(c, 0.f, -s, 0.f,
                               0.f, 1.f, 0.f, 0.f,
                               s, 0.f, c, 0.f,
                               0.f, 0.f, 0.f, 1.f);
  return Transform(mat, matInv);
}

Transform RotateZ(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  Matrix4x4 mat = Matrix4x4(c, -s, 0.f, 0.f,
                            s,  c, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f);
  Matrix4x4 matInv = Matrix4x4(c,  s, 0.f, 0.f,
                               -s, c, 0.f, 0.f,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f);
  return Transform(mat, matInv);
}


/**
 * Algorithm:
 * To rotate a vector v around an arbitrary axis, we can do the following:
 * - Let:
 *   + vector a be the normalized axis of rotation
 *   + vector v_c be the projection of v onto a => v_c = Dot(v, a) * a
 *   + vector v1 be (v - v_c) => v1 is perpendicular to a
 *   + vector v2 be (v1 x a)  => v2 is perpendicular to a and v1, and
 *     |v2| = |v1|
 * - Then we get a new coordinate system formed from a, v1, and v2.
 * - Since |v1| = |v2| and a, v1, v2 are pairwise perpendicular, we can
 *   perform a rotation around a and we obtain:
 *   + vector v1' = v1*cos(theta) - v2*sin(theta)
 *   + vector v'  = v_c + v1' is the result of rotating around the arbitrary
 * axis a to v
 *
 * Calculating matrices:
 * We apply the algorithm above to rotate the vectors i = (1, 0, 0),
 * j = (0, 1, 0), and k = (0, 0, 1):
 *  v | n | v[n] |   v_c[n] |   v1[n]   |              v2[n]
 * ---+---+------+----------+-----------+---------------------------------
 *  i | 0 |  1   | a.x*a.x  | 1-a.x*a.x | (0-a.x*a.y)*a.z-(0-a.x*a.z)*a.y
 *    | 1 |  0   | a.x*a.y  | 0-a.x*a.y | (0-a.x*a.z)*a.x-(1-a.x*a.x)*a.z
 *    | 2 |  0   | a.x*a.z  | 0-a.x*a.z | (1-a.x*a.x)*a.y-(0-a.x*a.y)*a.x
 * ---+---+------+----------+-----------+---------------------------------
 *  j | 0 |  0   | a.y*a.x  | 0-a.y*a.x | (1-a.y*a.y)*a.z-(0-a.y*a.z)*a.y
 *    | 1 |  1   | a.y*a.y  | 1-a.y*a.y | (0-a.y*a.z)*a.x-(0-a.y*a.x)*a.z
 *    | 2 |  0   | a.y*a.z  | 0-a.y*a.z | (0-a.y*a.x)*a.y-(1-a.y*a.y)*a.x
 * ---+---+------+----------+-----------+---------------------------------
 *  k | 0 |  0   | a.z*a.x  | 0-a.z*a.x | (0-a.z*a.y)*a.z-(1-a.z*a.z)*a.y
 *    | 1 |  0   | a.z*a.y  | 0-a.z*a.y | (1-a.z*a.z)*a.x-(0-a.z*a.x)*a.z
 *    | 2 |  1   | a.z*a.z  | 1-a.z*a.z | (0-a.z*a.x)*a.y-(0-a.z*a.y)*a.x
 * ---+---+------+----------+-----------+---------------------------------
 */
Transform Rotate(float angle, const Vector& axis) {
  // Normalize the axis
  Vector a = Normalize(axis);
  float c = cos(angle);
  float s = sin(angle);
  float m[4][4];

  m[0][0] = a.x * a.x * (1 - c) + c;
  m[1][0] = a.x * a.y * (1 - c) + a.z * s;
  m[2][0] = a.x * a.z * (1 - c) - a.y * s;
  m[3][0] = 0.f;

  // Rotate the y axis into this axis and do the rotation
  m[0][1] = a.y * a.x * (1 - c) - a.z* s;
  m[1][1] = a.y * a.y * (1 - c) + c;
  m[2][1] = a.y * a.z * (1 - c) + a.x * s;
  m[3][1] = 0.f;

  m[0][2] = a.z * a.x * (1 - c) + a.y * s;
  m[1][2] = a.z * a.y * (1 - c) - a.x * s;
  m[2][2] = a.z * a.z * (1 - c) + c;
  m[3][2] = 0.f;

  m[0][3] = 0.f;
  m[1][3] = 0.f;
  m[2][3] = 0.f;
  m[3][3] = 1.f;

  Matrix4x4 mat = Matrix4x4(m);
  return Transform(mat, Transpose(mat));
}

/*
Transform Scale(float x, float y, float z) {

}
*/

Transform AlignX(const Vector& v) {
  if (v.x == 0 && v.y == 0 && v.z == 0) {
    return Transform();
  }

  Vector v1, v2, v3;
  v1 = Normalize(v);
  CoordinateSystem(v1, &v2, &v3);

  Matrix4x4 mat = Matrix4x4(v1.x, v2.x, v3.x, 0,
                            v1.y, v2.y, v3.y, 0,
                            v1.z, v2.z, v3.z, 0,
                            0, 0, 0, 1);
  return Transform(Transpose(mat), mat);
}

Transform AlignY(const Vector& v) {
  if (v.x == 0 && v.y == 0 && v.z == 0) {
    return Transform();
  }

  Vector v1, v2, v3;
  v1 = Normalize(v);
  CoordinateSystem(v1, &v2, &v3);

  Matrix4x4 mat = Matrix4x4(v3.x, v1.x, v2.x, 0,
                            v3.y, v1.y, v2.y, 0,
                            v3.z, v1.z, v2.z, 0,
                            0, 0, 0, 1);
  return Transform(Transpose(mat), mat);
}

Transform AlignZ(const Vector& v) {
  if (v.x == 0 && v.y == 0 && v.z == 0) {
    return Transform();
  }

  Vector v1, v2, v3;
  v1 = Normalize(v);
  CoordinateSystem(v1, &v2, &v3);

  Matrix4x4 mat = Matrix4x4(v2.x, v3.x, v1.x, 0,
                            v2.y, v3.y, v1.y, 0,
                            v2.z, v3.z, v1.z, 0,
                            0, 0, 0, 1);
  return Transform(Transpose(mat), mat);
}


}  // namespace ishi

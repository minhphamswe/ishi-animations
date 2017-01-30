#include <core/point.h>
#include <core/vector.h>
#include <core/common.h>

namespace ishi {

Point::Point()
    : x(0), y(0), z(0) {}

Point::Point(float xx, float yy)
    : x(xx), y(yy), z(0) {}

Point::Point(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz) {}

Point Point::operator+(const Vector& v) const {
  return Point(x + v.x, y + v.y, z + v.z);
}

Point& Point::operator+=(const Vector& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Point Point::operator-(const Vector& v) const {
  return Point(x - v.x, y - v.y, z - v.z);
}

Point& Point::operator-=(const Vector& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector Point::operator-(const Point& p) const {
  return Vector(x - p.x, y - p.y, z - p.z);
}

Point Point::operator+(const Point& p) const {
  return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator/(float f) const {
  float inv = 1/f;
  return Point(x * inv, y * inv, z * inv);
}


Point Point::operator*(float f) const {
  return Point(x * f, y * f, z * f);
}

Point operator*(float f, const Point& p) {
  return Point(p.x * f, p.y * f, p.z * f);
}

bool Point::operator==(const Point& p) const {
  return ((x - p.x > -0.001) && (x - p.x < 0.001) &&
          (y - p.y > -0.001) && (y - p.y < 0.001) &&
          (z - p.z > -0.001) && (z - p.z < 0.001));
}

float& Point::operator[](int i) {
  Assert(i >= 0);
  Assert(i < 3);
  return (&x)[i];
}


float Point::operator[](int i) const {
  Assert(i >= 0);
  Assert(i < 3);
  return (&x)[i];
}

float Distance(const Point &p1, const Point &p2) {
  return Length(p2-p1);
}

}  // namespace ishi

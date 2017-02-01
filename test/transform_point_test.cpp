#include <catch/catch.hpp>
#include <core/transform.h>
#include <core/vector.h>
#include <core/point.h>

using namespace ishi;

// Verify identity transform does not affect either point
TEST_CASE("IdentityOnPoint", "[point][transform]") {
  Transform t = Transform();
  Point p = Point(5.1, 6.2, 7.3);
  CHECK(p == t(p));
}

// Verify inverse identity transform does not affect either point
TEST_CASE("InverseIdentityOnPoint", "[point][transform]") {
  Transform t = Inverse(Transform());
  Point p = Point(5.1, 6.2, 7.3);
  CHECK(p == t(p));
}

// Verify translation does move points
TEST_CASE("TranslationOnPoint", "[point][transform]") {
  Point p;
  Vector d;
  Transform t;

  p = Point(1, 2, 3);

  // Translate in X direction
  d = Vector(5.1, 0.f, 0.f);
  t = Translate(d);
  CHECK(t(p) == (p + d));

  // Translate in Y direction
  d = Vector(0.f, 6.2, 0.f);
  t = Translate(d);
  CHECK(t(p) == (p + d));

  // Translate in Z direction
  d = Vector(0.f, 0.f, 7.3);
  t = Translate(d);
  CHECK(t(p) == (p + d));

  // Translate in all 3 directions
  d = Vector(5.1, 6.2, 7.3);
  t = Translate(d);
  CHECK(t(p) == (p + d));
}

// Verify inverse translation does move a point
TEST_CASE("InverseTranslationOnPoint", "[point][transform]") {
  Point p;
  Vector d;
  Transform t;

  p = Point(1, 2, 3);

  // Translate in X direction
  d = Vector(5.1, 0.f, 0.f);
  t = Inverse(Translate(d));
  CHECK(t(p) == (p - d));

  // Translate in Y direction
  d = Vector(0.f, 6.2, 0.f);
  t = Inverse(Translate(d));
  CHECK(t(p) == (p - d));

  // Translate in Z direction
  d = Vector(0.f, 0.f, 7.3);
  t = Inverse(Translate(d));
  CHECK(t(p) == (p - d));

  // Translate in all 3 directions
  d = Vector(5.1, 6.2, 7.3);
  t = Inverse(Translate(d));
  CHECK(t(p) == (p - d));
}

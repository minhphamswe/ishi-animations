#include <catch/catch.hpp>
#include <core/transform.h>
#include <core/vector.h>
#include <core/point.h>

using namespace ishi;

/// Verify identity transform does not affect either point or vector
TEST_CASE("IdentityOnPointAndVector", "[vector][transform]") {
  Vector v = Vector(5.1, 6.2, 7.3);
  Transform t = Transform();
  CHECK(v == t(v));
}

/// Verify inverse identity transform does not affect either point or vector
TEST_CASE("InverseIdentityOnPointAndVector", "[vector][transform]") {
  Vector v = Vector(5.1, 6.2, 7.3);
  Transform t = Inverse(Transform());
  CHECK(v == t(v));
}

/// Verify translation does not affect vector
TEST_CASE("TranslationOnVector", "[vector][transform]") {
  Vector v, d;
  Transform t;

  v = Vector(1, 2, 3);

  // Translate in X direction
  d = Vector(5.1, 0.f, 0.f);
  t = Translate(d);
  CHECK(t(v) == v);

  // Translate in Y direction
  d = Vector(0.f, 6.2, 0.f);
  t = Translate(d);
  CHECK(t(v) == v);

  // Translate in Z direction
  d = Vector(0.f, 0.f, 7.3);
  t = Translate(d);
  CHECK(t(v) == v);

  // Translate in all 3 directions
  d = Vector(5.1, 6.2, 7.3);
  t = Translate(d);
  CHECK(t(v) == v);
}

/// Verify inverse translation does not affect vector
TEST_CASE("InverseTranslationOnVector", "[vector][transform]") {
  Vector v, d;
  Transform t;

  v = Vector(1, 2, 3);

  // Translate in X direction
  d = Vector(5.1, 0.f, 0.f);
  t = Inverse(Translate(d));
  CHECK(t(v) == v);

  // Translate in Y direction
  d = Vector(0.f, 6.2, 0.f);
  t = Inverse(Translate(d));
  CHECK(t(v) == v);

  // Translate in Z direction
  d = Vector(0.f, 0.f, 7.3);
  t = Inverse(Translate(d));
  CHECK(t(v) == v);

  // Translate in all 3 directions
  d = Vector(5.1, 6.2, 7.3);
  t = Inverse(Translate(d));
  CHECK(t(v) == v);
}

TEST_CASE("RotationXOnVector", "[vector][transform]") {
  Vector v = Vector(1, 2, 3);
}

TEST_CASE("ApplyInverseRotationX", "[vector][transform]") {
  
}

TEST_CASE("ApplyRotationY", "[vector][transform]") {
  
}

TEST_CASE("ApplyInverseRotationY", "[vector][transform]") {
  
}

TEST_CASE("ApplyRotationZ", "[vector][transform]") {
  
}

TEST_CASE("ApplyInverseRotationZ", "[vector][transform]") {
  
}

TEST_CASE("ApplyScale", "[vector][transform]") {
  
}

TEST_CASE("ApplyInverseScale", "[vector][transform]") {
  
}


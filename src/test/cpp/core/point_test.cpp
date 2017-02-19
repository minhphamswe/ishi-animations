#include <catch/catch.hpp>

#include <core/point.h>

using namespace ishi;

TEST_CASE("PointConstructors", "[point]") {
  Point p;
  // Default constructor returns origin
  p = Point();
  CHECK(p.x == 0);
  CHECK(p.y == 0);
  CHECK(p.z == 0);

  // Two arg constructor returns point on 0-plane
  for (float fx = -1.0f; fx < 1.0f; fx += 0.1f) {
    for (float fy = -1.0f; fy < 1.0f; fy += 0.1f) {
      p = Point(fx, fy);
      CHECK(p.x == fx);
      CHECK(p.y == fy);
      CHECK(p.z == 0);
    }
  }

  // Three arg constructor returns point in 3D space
  for (float fx = -1.0f; fx < 1.0f; fx += 0.1f) {
    for (float fy = -1.0f; fy < 1.0f; fy += 0.1f) {
      for (float fz = -1.0f; fz < 1.0f; fz += 0.1f) {
        p = Point(fx, fy, fz);
        CHECK(p.x == fx);
        CHECK(p.y == fy);
        CHECK(p.z == fz);
      }
    }
  }
}

TEST_CASE("PointEquality", "[point]") {
  Point p;

  p = Point(1, 0, 0);
  CHECK(p == p);
  CHECK(p == Point(1, 0, 0));
  CHECK(Point(1, 0, 0) == Point(1, 0, 0));

  p = Point(4, 5, 6);
  CHECK(p == p);
  CHECK(p == Point(4, 5, 6));
  CHECK(Point(4, 5, 6) == Point(4, 5, 6));
}

TEST_CASE("PointPointAddition", "[point]") {
  Point p1, p2, p;

  p1 = Point(1, 2, 3);
  p2 = Point(4, 5, 6);

  // Check addition identity
  p = p1 + Point(0, 0, 0);
  CHECK(p == Point(1, 2, 3));

  p = p2 + Point(0, 0, 0);
  CHECK(p == Point(4, 5, 6));

  // Check result of addition
  p = p1 + p2;
  CHECK(p == Point(5, 7, 9));

  // Check addition is commutative
  CHECK((p1 + p2) == (p2 + p1));
}

TEST_CASE("PointIndexAccessor", "[point]") {
  Point p;
  p = Point(5, 6);
  CHECK(p[0] == 5);
  CHECK(p[1] == 6);
  CHECK(p[2] == 0);

  p = Point(1, 2, 3);
  CHECK(p[0] == 1);
  CHECK(p[1] == 2);
  CHECK(p[2] == 3);
}



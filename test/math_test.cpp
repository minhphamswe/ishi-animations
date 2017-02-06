#include <catch/catch.hpp>

#include <core/math.h>

#include <cstdio>

TEST_CASE("feq() should be true after small floats divide/multiply", "[math]") {
  float a = 10.0f;
  float b = a / 7.0f * 7.0f;

  REQUIRE(feq(b, a));
  REQUIRE(feq(a, b));
}

TEST_CASE("feq() should be true after small floats subtract/add", "[math]") {
  float a = 10.0f;
  float b = a - 3.0f + 3.0f;

  REQUIRE(feq(b, a));
  REQUIRE(feq(a, b));
}

TEST_CASE("feq() should be false after small floats divide", "[math]") {
  float a = 10.0f;
  float b = a / 2.0f;

  REQUIRE(!feq(b, a));
  REQUIRE(!feq(a, b));
}

TEST_CASE("feq() should be false after small floats subtract", "[math]") {
  float a = 10.0f;
  float b = a - 1.0f;

  REQUIRE(!feq(b, a));
  REQUIRE(!feq(a, b));
}

TEST_CASE("feq() should be true for large floats divide/multiply", "[math]") {
  float a = std::numeric_limits<float>::max();
  float b = a / 7.0f * 7.0f;

  REQUIRE(feq(b, a));
  REQUIRE(feq(a, b));
}

TEST_CASE("feq() should be true after large floats subtract/add", "[math]") {
  float a = std::numeric_limits<float>::max();
  float b = a - 3.0f + 3.0f;

  REQUIRE(feq(b, a));
  REQUIRE(feq(a, b));
}

TEST_CASE("feq() should be false after large floats divide", "[math]") {
  float a = std::numeric_limits<float>::max();
  float b = a / 2.0f;

  REQUIRE(!feq(b, a));
  REQUIRE(!feq(a, b));
}

TEST_CASE("feq() should be false after large floats subtract", "[math]") {
  float a = 1e+6;

  float b = a - 1.0f;

  // Check that difference is not too small to be represented
  CHECK(a != b);

  REQUIRE(!feq(b, a));
  REQUIRE(!feq(a, b));
}
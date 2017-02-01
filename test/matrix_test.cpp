#include <catch/catch.hpp>
#include <core/matrix.h>

using namespace ishi;

TEST_CASE("Matrix4x4DefaultConstructor", "[matrix]") {
  Matrix4x4 mat;

  mat = Matrix4x4();
  CHECK(mat.m[0][0] == 1);
  CHECK(mat.m[0][1] == 0);
  CHECK(mat.m[0][2] == 0);
  CHECK(mat.m[0][3] == 0);

  CHECK(mat.m[1][0] == 0);
  CHECK(mat.m[1][1] == 1);
  CHECK(mat.m[1][2] == 0);
  CHECK(mat.m[1][3] == 0);

  CHECK(mat.m[2][0] == 0);
  CHECK(mat.m[2][1] == 0);
  CHECK(mat.m[2][2] == 1);
  CHECK(mat.m[2][3] == 0);

  CHECK(mat.m[3][0] == 0);
  CHECK(mat.m[3][1] == 0);
  CHECK(mat.m[3][2] == 0);
  CHECK(mat.m[3][3] == 1);
}

TEST_CASE("Matrix4x4ArrayConstructor", "[matrix]") {
  Matrix4x4 mat;
  float m[4][4] = {
    {1.f, 2.f, 3.f, 4.f},
    {5.f, 6.f, 7.f, 8.f},
    {1.5, 2.6, 3.7, 4.8},
    {5.1, 6.2, 7.3, 8.4}
  };

  mat = Matrix4x4(m);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      CHECK(mat.m[i][j] == Approx(m[i][j]).epsilon(0.00));
}

TEST_CASE("Matrix4x4FloatListConstructor", "[matrix]") {
  Matrix4x4 mat;
  float m[4][4] = {
    {1.f, 2.f, 3.f, 4.f},
    {5.f, 6.f, 7.f, 8.f},
    {1.5, 2.6, 3.7, 4.8},
    {5.1, 6.2, 7.3, 8.4}
  };

  mat = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                  5.f, 6.f, 7.f, 8.f,
                  1.5, 2.6, 3.7, 4.8,
                  5.1, 6.2, 7.3, 8.4);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      CHECK(mat.m[i][j] == Approx(m[i][j]).epsilon(0.00));
}

TEST_CASE("Matrix4x4EqualityOperator", "[matrix]") {
  Matrix4x4 m1, m2;

  float m[4][4] = {
    {1.f, 2.f, 3.f, 4.f},
    {5.f, 6.f, 7.f, 8.f},
    {1.5, 2.6, 3.7, 4.8},
    {5.1, 6.2, 7.3, 8.4}
  };

  m1 = Matrix4x4(m);
  m2 = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                 5.f, 6.f, 7.f, 8.f,
                 1.5, 2.6, 3.7, 4.8,
                 5.1, 6.2, 7.3, 8.4);

  CHECK(Matrix4x4() == Matrix4x4());
  CHECK(m1 == m2);
}

TEST_CASE("Matrix4x4InequalityOperator", "[matrix]") {
  Matrix4x4 m1, m2;
  float d = 5.7f;

  float m[4][4] = {
    {1.f, 2.f, 3.f, 4.f},
    {5.f, 6.f, 7.f, 8.f},
    {1.5, 2.6, 3.7, 4.8},
    {5.1, 6.2, 7.3, 8.4}
  };
  m1 = Matrix4x4(m);
  m2 = Matrix4x4(m);
  CHECK(!(m1 != m2));

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m2 = Matrix4x4(m);
      m2.m[i][j] = m2.m[i][j] + d;
      CHECK(m1 != m2);

      m2.m[i][j] = m2.m[i][j] - d;
      CHECK(!(m1 != m2));
      CHECK(m1.m[i][j] == Approx(m2.m[i][j]).epsilon(0.0001));
    }
  }
}

TEST_CASE("Matrix4x4Transpose", "[matrix]") {
  Matrix4x4 m1, m2;

  // Transpose of identity matrix is itself
  m1 = Matrix4x4();
  m2 = Matrix4x4();
  CHECK(m2 == Transpose(m1));

  // Check transpose for arbitrary matrix
  m1 = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                 5.f, 6.f, 7.f, 8.f,
                 1.5, 2.6, 3.7, 4.8,
                 5.1, 6.2, 7.3, 8.4);
  m2 = Matrix4x4(1.f, 5.f, 1.5, 5.1,
                 2.f, 6.f, 2.6, 6.2,
                 3.f, 7.f, 3.7, 7.3,
                 4.f, 8.f, 4.8, 8.4);
  CHECK(m2 == Transpose(m1));
}

TEST_CASE("Matrix4x4Multiply", "[matrix]") {
  Matrix4x4 i = Matrix4x4(), m1, m2, r;

  // Product of a matrix and an identity matrix is itself
  // Product of an identity matrix and a matrix is that matrix
  m1 = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                 5.f, 6.f, 7.f, 8.f,
                 1.5, 2.6, 3.7, 4.8,
                 5.1, 6.2, 7.3, 8.4);
  CHECK(Mul(m1, i) == m1);
  CHECK(Mul(i, m1) == m1);

  // The transpose of the product of 2 matrices is the product of the
  // transposes of these same matrices, multiplied in reverse order.
  m2 = Matrix4x4(1, 2, 3, 4,
                 5, 6, 7, 8,
                 9, 0, 1, 2,
                 3, 4, 5, 6);
  r = Matrix4x4(50., 30., 40., 50.,
                122., 78., 104., 130.,
                62.2, 37.8, 50.4, 63.,
                127., 81., 108., 135.);
  CHECK(Mul(m1, m2) == r);
  CHECK(Transpose(Mul(m1, m2)) == Mul(Transpose(m2), Transpose(m1)));
}

TEST_CASE("Matrix4x4Inverse", "[matrix]") {
  Matrix4x4 m1, m2, i;

  i = Matrix4x4();
  m2 = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                 5.f, 6.f, 7.f, 8.f,
                 1.5, 2.6, 3.7, 4.8,
                 5.1, 6.2, 7.3, 8.4);
  // Inverse of identity matrix is itself
  m1 = Matrix4x4();

  const Matrix4x4 &inv1 = Inverse(m1);
  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y) {
      CHECK(inv1.m[x][y] == Approx(i.m[x][y]).epsilon(0.00));
    }
  }

  // Check product of a matrix and its inverse is the identity matrix
  m1 = Matrix4x4(1.f, 2.f, 3.f, 4.f,
                 5.f, 6.f, 7.f, 8.f,
                 1.5, 2.6, 3.7, 4.8,
                 5.1, 6.2, 7.3, 8.4);
  m2 = Inverse(m1);

  const Matrix4x4 &mul2 = Mul(m1, m2);
  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y) {
      CHECK(mul2.m[x][y] == Approx(i.m[x][y]).epsilon(0.00));
    }
  }
}

/// Verify that we can access the underlying data through pointer arithmetic
TEST_CASE("ToFloatPointerWorks", "[matrix]") {
  float *fp;
  // Construct a floating point array
  float m[4][4];
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      m[i][j] = -i * 4 + j;

  // Verify that we can access elements in the array with pointer arithmetic
  fp = (float*) m;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      CHECK(m[i][j] == *(fp + i*4 + j));

  // Construct a transform from the array
  Matrix4x4 mat = Matrix4x4(m);

  // Verify that we can access elements in the transform with pointer arithmetic
  fp = (float*) mat.m;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      CHECK(mat.m[i][j] == *(fp + i*4 + j));
}

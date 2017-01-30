#include <core/matrix.h>

namespace ishi {

void swap_row(float (*m)[4][8], int r1, int r2) {
  float t;
  for (int j = 0; j < 8; j++) {
    t = (*m)[r1][j];
    (*m)[r1][j] = (*m)[r2][j];
    (*m)[r2][j] = t;
  }
}

void scale_row(float (*m)[4][8], int r, float f) {
  for (int j = 0; j < 8; j++)
    (*m)[r][j] = (*m)[r][j] * f;
}

void add_row(float (*m)[4][8], int from, int to, float f) {
  for (int j = 0; j < 8; j++)
    (*m)[to][j] = (*m)[to][j] + (*m)[from][j] * f;
}

Matrix4x4::Matrix4x4() {
  m[0][0] = 1.f;
  m[0][1] = 0.f;
  m[0][2] = 0.f;
  m[0][3] = 0.f;

  m[1][0] = 0.f;
  m[1][1] = 1.f;
  m[1][2] = 0.f;
  m[1][3] = 0.f;

  m[2][0] = 0.f;
  m[2][1] = 0.f;
  m[2][2] = 1.f;
  m[2][3] = 0.f;

  m[3][0] = 0.f;
  m[3][1] = 0.f;
  m[3][2] = 0.f;
  m[3][3] = 1.f;
}

Matrix4x4::Matrix4x4(float mat[4][4]) {
  m[0][0] = mat[0][0];
  m[0][1] = mat[0][1];
  m[0][2] = mat[0][2];
  m[0][3] = mat[0][3];

  m[1][0] = mat[1][0];
  m[1][1] = mat[1][1];
  m[1][2] = mat[1][2];
  m[1][3] = mat[1][3];

  m[2][0] = mat[2][0];
  m[2][1] = mat[2][1];
  m[2][2] = mat[2][2];
  m[2][3] = mat[2][3];

  m[3][0] = mat[3][0];
  m[3][1] = mat[3][1];
  m[3][2] = mat[3][2];
  m[3][3] = mat[3][3];
}

Matrix4x4::Matrix4x4(float a00, float a01, float a02, float a03,
                     float a10, float a11, float a12, float a13,
                     float a20, float a21, float a22, float a23,
                     float a30, float a31, float a32, float a33) {
  m[0][0] = a00;
  m[0][1] = a01;
  m[0][2] = a02;
  m[0][3] = a03;

  m[1][0] = a10;
  m[1][1] = a11;
  m[1][2] = a12;
  m[1][3] = a13;

  m[2][0] = a20;
  m[2][1] = a21;
  m[2][2] = a22;
  m[2][3] = a23;

  m[3][0] = a30;
  m[3][1] = a31;
  m[3][2] = a32;
  m[3][3] = a33;
}

bool Matrix4x4::operator==(const Matrix4x4 &mat) const {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if ((m[i][j] > mat.m[i][j] * 1.0001) || (m[i][j] < mat.m[i][j] * 0.9999))
        return false;
  return true;
}

bool Matrix4x4::operator!=(const Matrix4x4 &mat) const {
  return !(*this == mat);
}

/// This implementation uses the Gauss-Jordan elimination routine for
/// numerical stability
Matrix4x4 Inverse(const Matrix4x4 &mat) {
  Matrix4x4 I = Matrix4x4();
  float matI[4][8];

  // Augment matrix with the identity matrix
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matI[i][j] = mat.m[i][j];
      matI[i][j+4] = I.m[i][j];
    }
  }

  // Perform elementary row operations on the matrix until it reaches
  // reduced row echelon form
  for (int i = 0; i < 4; i++) {
    // Make sure the ith value in the ith row is not 0
    if (matI[i][i] == 0)
      for (int k = i; k < 4; k++)
        if (matI[k][i] != 0) {
          swap_row(&matI, i, k);
          break;
        }

    // TODO: if matrix is singular, the next statement will divide by 0

    // Scale the ith row so the ith value is 1
    if (matI[i][i] != 1)
      scale_row(&matI, i, 1/matI[i][i]);

    // Zero out the ith value in all the other rows
    for (int j = 0; j < 4; j++) {
      if (j != i) {
        add_row(&matI, i, j, -matI[j][i]);
      }
    }
  }

  // Extract and return matrix
  return Matrix4x4(matI[0][4], matI[0][5], matI[0][6], matI[0][7],
                   matI[1][4], matI[1][5], matI[1][6], matI[1][7],
                   matI[2][4], matI[2][5], matI[2][6], matI[2][7],
                   matI[3][4], matI[3][5], matI[3][6], matI[3][7]);
}

Matrix4x4 Transpose(const Matrix4x4 &mat) {
  return Matrix4x4(mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
                   mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
                   mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
                   mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

Matrix4x4 Mul(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 r;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      r.m[i][j] = m1.m[i][0] * m2.m[0][j] +
                  m1.m[i][1] * m2.m[1][j] +
                  m1.m[i][2] * m2.m[2][j] +
                  m1.m[i][3] * m2.m[3][j];
  return r;
}

}  // namespace ishi

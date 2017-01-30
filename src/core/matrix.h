#ifndef CORE_MATRIX_H_
#define CORE_MATRIX_H_

namespace ishi {

// Low-level matrix elementary row operations
void swap_row(float (*m)[4][8], int r1, int r2);
void scale_row(float (*m)[4][8], int r, float f);
void add_row(float (*m)[4][8], int from, int to, float f);


struct Matrix4x4 {
 public:
  float m[4][4];

 public:
  /// Initialize an identity matrix
  Matrix4x4();

  /// Initialize a matrix using an array
  explicit Matrix4x4(float mat[4][4]);

  /// Initialize a matrix using floats
  Matrix4x4(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33);

  /// Return true if this matrix is identical to the other matrix
  bool operator==(const Matrix4x4 &mat) const;

  /// Return false if this matrix is not identical to the other matrix
  bool operator!=(const Matrix4x4 &mat) const;
};

/// Return the transpose of the matrix
Matrix4x4 Transpose(const Matrix4x4 &mat);

/// Return the inverse of the matrix
Matrix4x4 Inverse(const Matrix4x4 &mat);

/// Multiply two matrices, returning a new matrix
Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2);

}  // namespace ishi

#endif

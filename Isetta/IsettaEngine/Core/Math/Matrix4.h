/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API Matrix4 {
 public:
  static const Matrix4 zero;
  static const Matrix4 identity;
  static const Matrix4 xRot45;
  static const Matrix4 xRot90;
  static const Matrix4 yRot45;
  static const Matrix4 yRot90;
  static const Matrix4 zRot45;
  static const Matrix4 zRot90;

  static const int ELEMENT_COUNT = 16;
  static const int ROW_COUNT = 4;

  union {
    float data[ELEMENT_COUNT];
    float row_col[ROW_COUNT][ROW_COUNT];
  };

  /**
   * \brief Default the constructor to create a zero matrix
   */
  Matrix4();
  /**
   * \brief Create a 4x4 matrix with the same value
   * \param value value of the matrix elements
   */
  explicit Matrix4(float value);
  // explicit Matrix4(const float* inMat);
  explicit Matrix4(const float inMat[]);
  Matrix4(float m11, float m12, float m13, float m14, float m21, float m22,
          float m23, float m24, float m31, float m32, float m33, float m34,
          float m41, float m42, float m43, float m44);
  explicit Matrix4(const class Quaternion& quat);

  Matrix4(const Matrix4& inMatrix);
  Matrix4(Matrix4&& inMatrix) noexcept;
  Matrix4& operator=(const Matrix4& inMatrix);
  Matrix4& operator=(Matrix4&& inMatrix) noexcept;

  Matrix4(const class Vector4& aVector, const class Vector4& bVector);

  ~Matrix4() {}

  // float operator[](int i) const;
  float* operator[](int i) const;
  bool operator==(const Matrix4& rhs) const;
  bool operator!=(const Matrix4& rhs) const;
  Matrix4 operator+(const Matrix4& rhs) const;
  Matrix4& operator+=(const Matrix4& rhs);
  Matrix4 operator-(const Matrix4& rhs) const;
  Matrix4 operator-=(const Matrix4& rhs);
  Matrix4 operator*(const Matrix4& rhs) const;
  Matrix4 operator*=(const Matrix4& rhs);
  Matrix4 operator*(float scalar) const;
  Matrix4 operator*=(float scalar);
  class Vector4 operator*(const class Vector4& rhs) const;

  /**
   * \brief Returns the determinant of this matrix
   */
  float Determinant() const;
  /**
   * \brief Get the value of a specific element
   * \param x Row index
   * \param y Column index
   */
  float Get(int x, int y) const;
  /**
   * \brief Set the value of a specific element
   * \param x Row index
   * \param y Column index
   * \param number New value of the element
   */
  void Set(int x, int y, float number);
  /**
   * \brief Returns the inverse matrix of the matrix
   */
  Matrix4 Inverse() const;
  /**
   * \brief Returns the transpose matrix of the matrix
   */
  Matrix4 Transpose() const;
  /**
   * \brief Returns true if the matrix is an identity matrix
   */
  bool IsIdentity() const;
  /**
   * \brief Return true if the matrix is a zero matrix
   */
  bool IsZero() const;
  /**
   * \brief Get the values of a specific row
   * \param row Row index
   */
  class Vector4 GetRow(int row) const;
  /**
   * \brief Set the values of a specific row
   * \param row Row data
   * \param rowData The new values of this row
   */
  void SetRow(int row, const class Vector4& rowData);
  void SetRow(int row, const class Vector3& rowData, float rowVal);
  /**
   * \brief Get the values of a specific column
   * \param col Column Index
   */
  class Vector4 GetCol(int col) const;

  class Matrix3 GetTopLeftMatrix3() const;
  /**
   * \brief Set the values of a specific column
   * \param col Column index
   * \param colData The values of this column
   */
  void SetCol(int col, const class Vector4& colData);
  void SetCol(int col, const class Vector3& colData, float colVal);
  void SetTopLeftMatrix3(const class Matrix3& matrix3);
  void SetDiagonal(const class Vector4& diagonal);
  void SetDiagonal(float r0c0, float r1c1, float r2c2, float r3c3);
  /**
   * \brief Get the translate matrix of the vector
   * \param translation The translation vector
   */
  static Matrix4 Translate(const class Vector3& translation);
  /**
   * \brief Get the scale matrix of the vector
   * \param scale The scale vector
   */
  static Matrix4 Scale(const class Vector3& scale);
  static Matrix4 RotateX(float radians);
  static Matrix4 RotateY(float radians);
  static Matrix4 RotateZ(float radians);
  static Matrix4 Rotate(const class Vector3& rotation);
  static Matrix4 Transform(const class Vector3& translation,
                           const class Vector3& rotation,
                           const class Vector3& scale);
  /**
   * \brief Checks if two matrix4 are equal (within a tolerance)
   * \param lhs Matrix A to be compared
   * \param rhs Matrix B to be compared
   */
  static bool FuzzyEqual(const Matrix4& lhs, const Matrix4& rhs);
};
}  // namespace Isetta::Math

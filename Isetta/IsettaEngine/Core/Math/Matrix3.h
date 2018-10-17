/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API_DECLARE Matrix3 {
 public:
  static const Matrix3 zero;
  static const Matrix3 identity;
  static const int ELEMENT_COUNT = 9;
  static const int ROW_COUNT = 3;

  union {
    float data[ELEMENT_COUNT];
    float row_col[ROW_COUNT][ROW_COUNT];
  };

  /**
   * \brief Default constructor to create a zero matrix
   */
  Matrix3();
  /**
   * \brief Create a 3x3 matrix with the same value
   * \param value value of the matrix elements
   */
  explicit Matrix3(float value);
  /**
   * \brief Create a 3x3 matrix by assigning each element
   */
  Matrix3(float m11, float m12, float m13, float m21, float m22, float m23,
          float m31, float m32, float m33);

  Matrix3(const Matrix3& inMatrix);
  Matrix3(Matrix3&& inMatrix) noexcept;
  Matrix3& operator=(const Matrix3& inMatrix);
  Matrix3& operator=(Matrix3&& inMatrix) noexcept;
  Matrix3(const class Vector3& aVector, const class Vector3& bVector);

  ~Matrix3() {}

  // float operator[](int i) const;
  float* operator[](int i) const;
  bool operator==(const Matrix3& rhs) const;
  bool operator!=(const Matrix3& rhs) const;
  Matrix3 operator+(const Matrix3& rhs) const;
  Matrix3& operator+=(const Matrix3& rhs);
  Matrix3 operator-(const Matrix3& rhs) const;
  Matrix3 operator-=(const Matrix3& rhs);
  Matrix3 operator*(const Matrix3& rhs) const;
  Matrix3 operator*=(const Matrix3& rhs);
  Matrix3 operator*(float scalar) const;
  Matrix3 operator*=(float scalar);
  class Vector3 operator*(const class Vector3& rhs) const;

  /**
   * \brief Returns the determinant of this matrix
   */
  float Determinant() const;
  /**
   * \brief Get the value of a specific element
   * \param x row index
   * \param y column index
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
   * \brief Returns the inverse matrix of this matrix
   */
  Matrix3 Inverse() const;
  /**
   * \brief Returns the transpose matrix of this matrix
   */
  Matrix3 Transpose() const;
  /**
   * \brief Returns true if the matrix is an identity matrix
   */
  bool IsIdentity() const;
  /**
   * \brief Returns true if the matrix is a zero matrix
   */
  bool IsZero() const;
  /**
   * \brief Get the values of a specific row
   * \param row Row index
   */
  class Vector3 GetRow(int row) const;
  /**
   * \brief Set the values of a specific row
   * \param row Row index
   * \param rowData The new values of this row
   */
  void SetRow(int row, class Vector3 rowData);
  /**
   * \brief Get the values of a specific column
   * \param col Column index
   */
  class Vector3 GetCol(int col) const;
  /**
   * \brief Set the values of a specific column
   * \param col Column index
   * \param colData The values of this column
   */
  void SetCol(int col, class Vector3 colData);
  void SetDiagonal(const Vector3& diagonal);
  void SetDiagonal(float r0c0, float r1c1, float r2c2);
  /**
   * \brief Checks if two matrix3 are equal (within a tolerance)
   * \param lhs Matrix A to be compared
   * \param rhs Matrix B to be compared
   */
  static bool FuzzyEqual(const Matrix3& lhs, const Matrix3& rhs);
};
}  // namespace Isetta::Math

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {
class Matrix3 {
 private:
  float data[9];

 public:
  static const Matrix3 zero;
  static const Matrix3 identity;

  Matrix3();
  explicit Matrix3(float value);
  Matrix3(float m11, float m12, float m13, float m21, float m22, float m23,
          float m31, float m32, float m33);

  Matrix3(const Matrix3& inMatrix);
  Matrix3(Matrix3&& inMatrix);
  Matrix3& operator=(const Matrix3& inMatrix);
  Matrix3& operator=(Matrix3&& inMatrix);

  Matrix3(const class Vector3& aVector, const class Vector3& bVector);

  ~Matrix3() {}

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

  float Determinant() const;
  float Get(int x, int y) const;
  void Set(int x, int y, float number);
  Matrix3 Inverse() const;
  Matrix3 Transpose() const;
  bool IsIdentity() const;
  bool IsZero() const;
  class Vector3 GetRow(int row) const;
  void SetRow(int row, class Vector3 rowData);
  class Vector3 GetCol(int col) const;
  void SetCol(int col, class Vector3 colData);
};
}  // namespace Isetta::Math

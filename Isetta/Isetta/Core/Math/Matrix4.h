/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {
class Matrix4 {
 private:
  float data[16];

 public:
  static const Matrix4 zero;
  static const Matrix4 identity;

  Matrix4();
  explicit Matrix4(float value);
  Matrix4(float m11, float m12, float m13, float m14, float m21, float m22,
          float m23, float m24, float m31, float m32, float m33, float m34,
          float m41, float m42, float m43, float m44);

  Matrix4(const Matrix4& inMatrix);
  Matrix4(Matrix4&& inMatrix);
  Matrix4& operator=(const Matrix4& inMatrix);
  Matrix4& operator=(Matrix4&& inMatrix);

  Matrix4(const class Vector4& aVector, const class Vector4& bVector);

  ~Matrix4() {}

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

  float Determinant() const;
  float Get(int x, int y) const;
  void Set(int x, int y, float number);
  Matrix4 Inverse() const;
  Matrix4 Transpose() const;
  bool IsIdentity() const;
  bool IsZero() const;
  class Vector4 GetRow(int row) const;
  void SetRow(int row, class Vector4 rowData);
  class Vector4 GetCol(int col) const;
  void SetCol(int col, class Vector4 colData);

  static Matrix4 Translate(const class Vector3& translation);
  static Matrix4 Scale(const class Vector3 scale);
};
}  // namespace Isetta::Math

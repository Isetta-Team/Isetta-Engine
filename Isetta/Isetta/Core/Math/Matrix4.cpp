/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Matrix4.h"

#include <cstring>
#include <stdexcept>
#include <utility>
#include "Vector3.h"
#include "Vector4.h"

namespace Isetta::Math {
const Matrix4 Matrix4::zero = Matrix4{};
const Matrix4 Matrix4::identity =
    Matrix4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

Matrix4::Matrix4() { memset(data, 0, sizeof(data)); }

Matrix4::Matrix4(float value) {
  for (float& item : data) {
    item = value;
  }
}

Matrix4::Matrix4(float m11, float m12, float m13, float m14, float m21,
                 float m22, float m23, float m24, float m31, float m32,
                 float m33, float m34, float m41, float m42, float m43,
                 float m44) {
  data[0] = m11;
  data[1] = m12;
  data[2] = m13;
  data[3] = m14;
  data[4] = m21;
  data[5] = m22;
  data[6] = m23;
  data[7] = m24;
  data[8] = m31;
  data[9] = m32;
  data[10] = m33;
  data[11] = m34;
  data[12] = m41;
  data[13] = m42;
  data[14] = m43;
  data[15] = m44;
}

Matrix4::Matrix4(const Matrix4& inMatrix) {
  for (int i = 0; i < 16; i++) {
    data[i] = inMatrix.data[i];
  }
}

Matrix4::Matrix4(Matrix4&& inMatrix) {
  for (int i = 0; i < 16; i++) {
    data[i] = inMatrix.data[i];
  }
}

Matrix4& Matrix4::operator=(const Matrix4& inMatrix) {
  for (int i = 0; i < 16; i++) {
    data[i] = inMatrix.data[i];
  }
  return *this;
}

Matrix4& Matrix4::operator=(Matrix4&& inMatrix) {
  for (int i = 0; i < 16; i++) {
    data[i] = inMatrix.data[i];
  }
  return *this;
}

Matrix4::Matrix4(const Vector4& aVector, const Vector4& bVector) {
  data[0] = aVector.x * bVector.x;
  data[1] = aVector.x * bVector.y;
  data[2] = aVector.x * bVector.z;
  data[3] = aVector.x * bVector.w;
  data[4] = aVector.y * bVector.x;
  data[5] = aVector.y * bVector.y;
  data[6] = aVector.y * bVector.z;
  data[7] = aVector.y * bVector.w;
  data[8] = aVector.z * bVector.x;
  data[9] = aVector.z * bVector.y;
  data[10] = aVector.z * bVector.z;
  data[11] = aVector.z * bVector.w;
  data[12] = aVector.w * bVector.x;
  data[13] = aVector.w * bVector.y;
  data[14] = aVector.w * bVector.z;
  data[15] = aVector.w * bVector.w;
}

bool Matrix4::operator==(const Matrix4& rhs) const {
  for (int i = 0; i < 16; i++) {
    if (data[i] != rhs.data[i]) {
      return false;
    }
  }
  return true;
}

bool Matrix4::operator!=(const Matrix4& rhs) const {
  for (int i = 0; i < 16; i++) {
    if (data[i] != rhs.data[i]) {
      return true;
    }
  }
  return false;
}

Matrix4 Matrix4::operator+(const Matrix4& rhs) const {
  Matrix4 ret{};
  for (int i = 0; i < 16; i++) {
    ret.data[i] += rhs.data[i];
  }
  return ret;
}

Matrix4& Matrix4::operator+=(const Matrix4& rhs) {
  for (int i = 0; i < 16; i++) {
    data[i] += rhs.data[i];
  }
  return *this;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const {
  Matrix4 ret{};
  for (int i = 0; i < 16; i++) {
    ret.data[i] -= rhs.data[i];
  }
  return ret;
}

Matrix4 Matrix4::operator-=(const Matrix4& rhs) {
  for (int i = 0; i < 16; i++) {
    data[i] += rhs.data[i];
  }
  return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
  Matrix4 ret{};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        ret.data[(i << 2) + j] += data[(i << 2) + k] * rhs.data[(k << 2) + j];
      }
    }
  }
  return ret;
}

Matrix4 Matrix4::operator*=(const Matrix4& rhs) {
  float newData[16];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        newData[(i << 2) + j] += data[(i << 2) + k] * rhs.data[(k << 2) + j];
      }
    }
  }
  for (int i = 0; i < 16; i++) {
    data[i] = newData[i];
  }
  return *this;
}

Matrix4 Matrix4::operator*(float scalar) const {
  Matrix4 ret{};
  for (int i = 0; i < 16; i++) {
    ret.data[i] = data[i] * scalar;
  }
  return ret;
}

Matrix4 Matrix4::operator*=(float scalar) {
  for (int i = 0; i < 16; i++) {
    data[i] *= scalar;
  }
  return *this;
}

Vector4 Matrix4::operator*(const Vector4& rhs) const {
  return Vector4{
      data[0] * rhs.x + data[1] * rhs.y + data[2] * rhs.z + data[3] * rhs.w,
      data[4] * rhs.x + data[5] * rhs.y + data[6] * rhs.z + data[7] * rhs.w,
      data[8] * rhs.x + data[9] * rhs.y + data[10] * rhs.z + data[11] * rhs.w,
      data[12] * rhs.x + data[13] * rhs.y + data[14] * rhs.z +
          data[15] * rhs.w};
}

float Matrix4::Determinant() const {
  float m11 = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] -
              data[9] * data[6] * data[15] + data[9] * data[7] * data[14] +
              data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
  float m21 = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] +
              data[8] * data[6] * data[15] - data[8] * data[7] * data[14] -
              data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
  float m31 = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] -
              data[8] * data[5] * data[15] + data[8] * data[7] * data[13] +
              data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
  float m41 = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] +
              data[8] * data[5] * data[14] - data[8] * data[6] * data[13] -
              data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
  return data[0] * m11 + data[1] * m21 + data[2] * m31 + data[3] * m41;
}

float Matrix4::Get(int x, int y) const {
  if (x > 3 || x < 0 || y > 3 || y < 0) {
    throw std::out_of_range("Matrix4::Get: Matrix index out of range.");
  }
  return data[(x << 2) + y];
}

void Matrix4::Set(int x, int y, float number) {
  if (x > 3 || x < 0 || y > 3 || y < 0) {
    throw std::out_of_range("Matrix4::Set: Matrix index out of range.");
  }
  data[(x << 2) + y] = number;
}

Matrix4 Matrix4::Inverse() const {
  float m11 = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] -
              data[9] * data[6] * data[15] + data[9] * data[7] * data[14] +
              data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
  float m21 = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] +
              data[8] * data[6] * data[15] - data[8] * data[7] * data[14] -
              data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
  float m31 = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] -
              data[8] * data[5] * data[15] + data[8] * data[7] * data[13] +
              data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
  float m41 = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] +
              data[8] * data[5] * data[14] - data[8] * data[6] * data[13] -
              data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
  float m12 = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] +
              data[9] * data[2] * data[15] - data[9] * data[3] * data[14] -
              data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
  float m22 = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] -
              data[8] * data[2] * data[15] + data[8] * data[3] * data[14] +
              data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
  float m32 = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] +
              data[8] * data[1] * data[15] - data[8] * data[3] * data[13] -
              data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
  float m42 = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] -
              data[8] * data[1] * data[14] + data[8] * data[2] * data[13] +
              data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
  float m13 = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] -
              data[5] * data[2] * data[15] + data[5] * data[3] * data[14] +
              data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
  float m23 = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] +
              data[4] * data[2] * data[15] - data[4] * data[3] * data[14] -
              data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
  float m33 = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] -
              data[4] * data[1] * data[15] + data[4] * data[3] * data[13] +
              data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
  float m43 = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] +
              data[4] * data[1] * data[14] - data[4] * data[2] * data[13] -
              data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
  float m14 = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] +
              data[5] * data[2] * data[11] - data[5] * data[3] * data[10] -
              data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
  float m24 = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] -
              data[4] * data[2] * data[11] + data[4] * data[3] * data[10] +
              data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
  float m34 = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] +
              data[4] * data[1] * data[11] - data[4] * data[3] * data[9] -
              data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
  float m44 = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] -
              data[4] * data[1] * data[10] + data[4] * data[2] * data[9] +
              data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

  float det = data[0] * m11 + data[1] * m21 + data[2] * m31 + data[3] * m41;

  if (det == 0) {
    throw std::out_of_range(
        "Matrix4::Inverse: Cannot do inverse when the determinant is zero.");
  }

  Matrix4 ret{m11, m12, m13, m14, m21, m22, m23, m24,
              m31, m32, m33, m34, m41, m42, m43, m44};
  return ret * (1.f * det);
}

Matrix4 Matrix4::Transpose() const {
  Matrix4 ret{*this};
  std::swap(ret.data[1], ret.data[4]);
  std::swap(ret.data[2], ret.data[8]);
  std::swap(ret.data[3], ret.data[12]);
  std::swap(ret.data[6], ret.data[9]);
  std::swap(ret.data[7], ret.data[13]);
  std::swap(ret.data[11], ret.data[14]);
  return ret;
}

bool Matrix4::IsIdentity() const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (data[(i << 2) + j] != static_cast<float>(i == j) ||
          data[(j << 2) + i] != static_cast<float>(i == j)) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix4::IsZero() const {
  for (int i = 0; i < 16; i++) {
    if (data[i] != 0) return false;
  }
  return true;
}

Vector4 Matrix4::GetRow(int row) const {
  if (row < 0 || row > 3)
    throw std::out_of_range("Matrix4::GetRow: Row index out of range.");
  return Vector4(data[row << 2], data[(row << 2) + 1], data[(row << 2) + 2],
                 data[(row << 2) + 3]);
}

void Matrix4::SetRow(int row, Vector4 rowData) {
  if (row < 0 || row > 2)
    throw std::out_of_range("Matrix4::SetRow: Row index out of range.");
  data[row << 2] = rowData.x;
  data[(row << 2) + 1] = rowData.y;
  data[(row << 2) + 2] = rowData.z;
  data[(row << 2) + 3] = rowData.w;
}

Vector4 Matrix4::GetCol(int col) const {
  if (col < 0 || col > 3)
    throw std::out_of_range("Matrix4:GetCol: Column index out of range.");
  return Vector4(data[col], data[4 + col], data[8 + col], data[12 + col]);
}

void Matrix4::SetCol(int col, Vector4 colData) {
  if (col < 0 || col > 3)
    throw std::out_of_range("Matrix4:SetCol: Column index out of range.");
  data[col] = colData.x;
  data[4 + col] = colData.y;
  data[8 + col] = colData.z;
  data[12 + col] = colData.w;
}

Matrix4 Matrix4::Translate(const Vector3& translation) {
  return Matrix4{1, 0, 0, translation.x, 0, 1, 0, translation.y,
                 0, 0, 1, translation.z, 0, 0, 0, 1};
}

Matrix4 Matrix4::Scale(const Vector3 scale) {
  return Matrix4{scale.x, 0, 0,       0, 0, scale.y, 0, 0,
                 0,       0, scale.z, 0, 0, 0,       0, 1};
}
}  // namespace Isetta::Math

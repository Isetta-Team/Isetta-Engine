/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Matrix3.h"

#include <cstring>
#include <stdexcept>
#include <utility>
#include "Vector3.h"

namespace Isetta::Math {
const Matrix3 Matrix3::zero = Matrix3{};
const Matrix3 Matrix3::identity = Matrix3{1, 0, 0, 0, 1, 0, 0, 0, 1};

Matrix3::Matrix3() { memset(data, 0, sizeof(data)); }

Matrix3::Matrix3(float value) {
  for (float& item : data) {
    item = value;
  }
}

Matrix3::Matrix3(float m11, float m12, float m13, float m21, float m22,
                 float m23, float m31, float m32, float m33) {
  data[0] = m11;
  data[1] = m12;
  data[2] = m13;
  data[3] = m21;
  data[4] = m22;
  data[5] = m23;
  data[6] = m31;
  data[7] = m32;
  data[8] = m33;
}

Matrix3::Matrix3(const Matrix3& inMatrix) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) data[i] = inMatrix.data[i];
}

Matrix3::Matrix3(Matrix3&& inMatrix) noexcept {
  for (int i = 0; i < ELEMENT_COUNT; ++i) data[i] = inMatrix.data[i];
}

// float Matrix3::operator[](int i) const {
//  if (i < 0 || i > ELEMENT_COUNT - 1)
//    throw std::out_of_range{"Matrix3::[] => Index access out of range."};
//  return data[i];
//}
float* Matrix3::operator[](int i) const {
  if (i < 0 || i > ROW_COUNT - 1)
    throw std::out_of_range{"Matrix3::[] => Index access out of range."};
  return (float*)(row_col[i]);
}
Matrix3& Matrix3::operator=(const Matrix3& inMatrix) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) data[i] = inMatrix.data[i];
  return *this;
}
Matrix3& Matrix3::operator=(Matrix3&& inMatrix) noexcept {
  for (int i = 0; i < ELEMENT_COUNT; ++i) data[i] = inMatrix.data[i];
  return *this;
}
Matrix3::Matrix3(const Vector3& aVector, const Vector3& bVector) {
  data[0] = aVector.x * bVector.x;
  data[1] = aVector.x * bVector.y;
  data[2] = aVector.x * bVector.z;
  data[3] = aVector.y * bVector.x;
  data[4] = aVector.y * bVector.y;
  data[5] = aVector.y * bVector.z;
  data[6] = aVector.z * bVector.x;
  data[7] = aVector.z * bVector.y;
  data[8] = aVector.z * bVector.z;
}

bool Matrix3::operator==(const Matrix3& rhs) const {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    if (data[i] != rhs.data[i]) {
      return false;
    }
  }
  return true;
}
bool Matrix3::operator!=(const Matrix3& rhs) const { return !(*this == rhs); }
Matrix3 Matrix3::operator+(const Matrix3& rhs) const {
  Matrix3 ret{};
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    ret.data[i] += rhs.data[i];
  }
  return ret;
}
Matrix3& Matrix3::operator+=(const Matrix3& rhs) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    data[i] += rhs.data[i];
  }
  return *this;
}
Matrix3 Matrix3::operator-(const Matrix3& rhs) const {
  Matrix3 ret{};
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    ret.data[i] -= rhs.data[i];
  }
  return ret;
}
Matrix3 Matrix3::operator-=(const Matrix3& rhs) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    data[i] += rhs.data[i];
  }
  return *this;
}
Matrix3 Matrix3::operator*(const Matrix3& rhs) const {
  Matrix3 ret{};
  for (int i = 0; i < ROW_COUNT; ++i) {
    for (int j = 0; j < ROW_COUNT; ++j) {
      for (int k = 0; k < ROW_COUNT; ++k) {
        ret.data[i * 3 + j] += data[i * 3 + k] * rhs.data[k * 3 + j];
      }
    }
  }
  return ret;
}
Matrix3 Matrix3::operator*=(const Matrix3& rhs) {
  float newData[ROW_COUNT][ROW_COUNT];
  for (int i = 0; i < ROW_COUNT; ++i) {
    for (int j = 0; j < ROW_COUNT; ++j) {
      newData[i][j] = 0;
      for (int k = 0; k < ROW_COUNT; ++k) {
        newData[i][j] += row_col[i][k] * rhs.row_col[k][j];
      }
    }
  }
  for (int i = 0; i < ROW_COUNT; ++i) {
    for (int j = 0; j < ROW_COUNT; ++j) {
      row_col[i][j] = newData[i][j];
    }
  }
  return *this;
}
Matrix3 Matrix3::operator*(float scalar) const {
  Matrix3 ret{};
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    ret.data[i] = data[i] * scalar;
  }
  return ret;
}
Matrix3 Matrix3::operator*=(float scalar) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    data[i] *= scalar;
  }
  return *this;
}
Vector3 Matrix3::operator*(const Vector3& rhs) const {
  return Vector3(data[0] * rhs.x + data[1] * rhs.y + data[2] * rhs.z,
                 data[3] * rhs.x + data[4] * rhs.y + data[5] * rhs.z,
                 data[6] * rhs.x + data[7] * rhs.y + data[8] * rhs.z);
}
float Matrix3::Determinant() const {
  return data[0] * (data[4] * data[8] - data[7] * data[5]) -
         data[1] * (data[3] * data[8] - data[6] * data[5]) +
         data[2] * (data[3] * data[7] - data[6] * data[4]);
}
float Matrix3::Get(int x, int y) const {
  if (x > ELEMENT_COUNT - 1 || x < 0 || y > ELEMENT_COUNT - 1 || y < 0) {
    throw std::out_of_range{"Matrix3::Get => Matrix index out of range."};
  }
  return row_col[x][y];
}
void Matrix3::Set(int x, int y, float number) {
  if (x > ELEMENT_COUNT - 1 || x < 0 || y > ELEMENT_COUNT - 1 || y < 0) {
    throw std::out_of_range{"Matrix3::Set => Matrix index out of range."};
  }
  row_col[x][y] = number;
}
Matrix3 Matrix3::Inverse() const {
  float det = Determinant();
  if (det == 0) {
    throw std::logic_error{
        "Matrix3::Inverse => Cannot do inverse when the determinant is zero."};
  }
  Matrix3 ret{};
  Matrix3 t{Transpose()};
  Matrix3 adj{t.data[4] * t.data[8] - t.data[5] * t.data[7],
              -t.data[3] * t.data[8] + t.data[5] * t.data[6],
              t.data[3] * t.data[7] - t.data[4] * t.data[6],
              -t.data[1] * t.data[8] + t.data[2] * t.data[7],
              t.data[0] * t.data[8] - t.data[2] * t.data[6],
              -t.data[0] * t.data[7] + t.data[1] * t.data[6],
              t.data[1] * t.data[5] - t.data[2] * t.data[4],
              -t.data[0] * t.data[5] + t.data[2] * t.data[3],
              t.data[0] * t.data[4] - t.data[1] * t.data[3]};
  return adj * (1.f / det);
}
Matrix3 Matrix3::Transpose() const {
  Matrix3 ret{*this};
  std::swap(ret.data[1], ret.data[3]);
  std::swap(ret.data[2], ret.data[6]);
  std::swap(ret.data[5], ret.data[7]);
  return ret;
}

bool Matrix3::IsIdentity() const {
  for (int i = 0; i < ROW_COUNT; ++i) {
    for (int j = 0; j < ROW_COUNT; ++j) {
      if (data[i * ROW_COUNT + j] != static_cast<float>(i == j) ||
          data[j * ROW_COUNT + i] != static_cast<float>(i == j)) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix3::IsZero() const {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    if (data[i] != 0) return false;
  }
  return true;
}

Vector3 Matrix3::GetRow(int row) const {
  if (row < 0 || row > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Matrix3::GetRow => Row index out of range."};
  return Vector3(data[row * ROW_COUNT], data[row * ROW_COUNT + 1],
                 data[row * ROW_COUNT + 2]);
}

void Matrix3::SetRow(int row, Vector3 rowData) {
  if (row < 0 || row > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Matrix3::SetRow => Row index out of range."};
  data[row * ROW_COUNT] = rowData.x;
  data[row * ROW_COUNT + 1] = rowData.y;
  data[row * ROW_COUNT + 2] = rowData.z;
}

Vector3 Matrix3::GetCol(int col) const {
  if (col < 0 || col > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Matrix3::GetCol => Column index out of range."};
  return Vector3(data[col], data[ROW_COUNT + col], data[ROW_COUNT * 2 + col]);
}

void Matrix3::SetCol(int col, Vector3 colData) {
  if (col < 0 || col > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Matrix3::SetCol => Column index out of range."};
  data[col] = colData.x;
  data[ROW_COUNT + col] = colData.y;
  data[ROW_COUNT * 2 + col] = colData.z;
}

void Matrix3::SetDiagonal(const Vector3& diagonal) {
  SetDiagonal(diagonal.x, diagonal.y, diagonal.z);
}

void Matrix3::SetDiagonal(const float r0c0, const float r1c1,
                          const float r2c2) {
  Set(0, 0, r0c0);
  Set(1, 1, r1c1);
  Set(2, 2, r2c2);
}

bool Matrix3::FuzzyEqual(const Matrix3& lhs, const Matrix3& rhs) {
  for (int i = 0; i < ELEMENT_COUNT; ++i) {
    if (abs(lhs.data[i] - rhs.data[i]) > FLT_EPSILON) {
      return false;
    }
  }
  return true;
}
}  // namespace Isetta::Math

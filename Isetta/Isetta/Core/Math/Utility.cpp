/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Utility.h"

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace Isetta::Math {
const float Utility::PI = static_cast<float>(M_PI);
const float Utility::EPSILON = FLT_EPSILON;
const float Utility::DEG2RAD = Utility::PI / 180.f;
const float Utility::RAD2DEG = 180.f * static_cast<float>(M_1_PI);
float Utility::Acos(float number) { return acosf(number); }
float Utility::Asin(float number) { return asinf(number); }
float Utility::Atan(float number) { return atanf(number); }
float Utility::Atan2(float y, float x) { return atan2f(y, x); }
float Utility::Ceil(float number) { return ceilf(number); }
int Utility::CellToInt(float number) { return static_cast<int>(ceilf(number)); }
float Utility::Clamp(float start, float end, float number) {
  return number < start ? start : (number > end ? end : number);
}
int Utility::Clamp(int start, int end, int number) {
  return number < start ? start : (number > end ? end : number);
}
float Utility::Clamp01(float number) {
  return Utility::Clamp(0.f, 1.f, number);
}
int Utility::ClosestPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range(
        "Utility::ClosestPowerOfTwo: Negative numbers are not supported.");
  int ceil = number - 1;
  for (size_t i = 1; i < sizeof(ceil) * CHAR_BIT; i *= 2) {
    ceil |= ceil >> i;
  }
  ceil++;
  int floor = ceil >> 1;
  if (abs(number - ceil) < abs(number - floor)) {
    return ceil;
  } else {
    return floor;
  }
}
float Utility::Cos(float radian) { return cosf(radian); }
float Utility::Exp(float power) { return expf(power); }
float Utility::Floor(float number) { return floorf(number); }
int Utility::FloorToInt(float number) {
  return static_cast<int>(floorf(number));
}
float Utility::InverseLerp(float start, float end, float number) {
  return (number - start) / (end - start);
}
bool Utility::IsPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range(
        "Utility::IsPowerOfTwo: Negative numbers are not supported.");
  return !(number == 0) && !(number & (number - 1));
}
float Utility::Lerp(float start, float end, float time) {
  return start * (1 - time) + time * end;
}
float Utility::Ln(float number) { return logf(number); }
float Utility::Log(float number, float base) {
  return logf(number) / log(base);
}
float Utility::Log10(float number) { return log10f(number); }
float Utility::Max(std::initializer_list<float> numbers) {
  return std::max(numbers);
}
int Utility::Max(std::initializer_list<int> numbers) {
  return std::max(numbers);
}
float Utility::Min(std::initializer_list<float> numbers) {
  return std::min(numbers);
}
int Utility::Min(std::initializer_list<int> numbers) {
  return std::min(numbers);
}

float Utility::MoveTowards(float current, float target, float maxDelta) {
  return std::min(current + maxDelta, target);
}

int Utility::NextPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range(
        "Utility::NextPowerOfTwo: Negative numbers are not supported.");
  int ceil = number - 1;
  for (size_t i = 1; i < sizeof(ceil) * CHAR_BIT; i *= 2) {
    ceil |= ceil >> i;
  }
  ceil++;
  return ceil;
}
float Utility::Pow(float number, float power) { return powf(number, power); }
float Utility::Repeat(float t, float length) {
  return t - floorf(t / length) * length;
}
float Utility::Round(float number) { return roundf(number); }
int Utility::RoundToInt(float number) {
  return static_cast<int>(roundf(number));
}
int Utility::Sign(float number) { return (number > 0) - (number < 0); }
float Utility::Sin(float radian) { return sinf(radian); }
float Utility::SmoothStep(float start, float end, float number) {
  number = Utility::Clamp01((number - start) / (end - start));
  return Square(number) * (3 - 2 * number);
}
float Utility::Sqrt(float number) { return sqrtf(number); }
float Utility::Square(float number) { return number * number; }
int Utility::Square(int number) { return number * number; }
float Utility::Tan(float radian) { return tanf(radian); }
}  // namespace Isetta::Math

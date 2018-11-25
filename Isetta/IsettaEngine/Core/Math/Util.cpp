/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Util.h"

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "Core/IsettaAlias.h"
//#include "Util.h"

namespace Isetta::Math {
const float Util::PI = static_cast<float>(M_PI);
const float Util::PI_HALF = 0.5f * PI;
const float Util::EPSILON = FLT_EPSILON;
const float Util::DEG2RAD = Util::PI / 180.f;
const float Util::RAD2DEG = 180.f * static_cast<float>(M_1_PI);

float Util::Abs(float number) { return number < 0 ? -number : number; }

int Util::Abs(int number) { return number < 0 ? -number : number; }

float Util::Acos(float number) { return acosf(number); }
float Util::Asin(float number) { return asinf(number); }
float Util::Atan(float number) { return atanf(number); }
float Util::Atan2(float y, float x) { return atan2f(y, x); }
float Util::Ceil(float number) { return ceilf(number); }
int Util::CeilToInt(float number) { return static_cast<int>(ceilf(number)); }
float Util::Clamp(float start, float end, float number) {
  return number < start ? start : (number > end ? end : number);
}
int Util::Clamp(int start, int end, int number) {
  return number < start ? start : (number > end ? end : number);
}
float Util::Clamp01(float number) { return Util::Clamp(0.f, 1.f, number); }
int Util::ClosestPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range{
        "Utility::ClosestPowerOfTwo => Negative numbers are not supported."};
  int ceil = number - 1;
  for (Size i = 1; i < sizeof(ceil) * CHAR_BIT; i *= 2) {
    ceil |= ceil >> i;
  }
  ++ceil;
  int floor = ceil >> 1;
  if (abs(number - ceil) < abs(number - floor)) {
    return ceil;
  } else {
    return floor;
  }
}
float Util::Cos(float radian) { return cosf(radian); }
float Util::Exp(float power) { return expf(power); }
float Util::Floor(float number) { return floorf(number); }
int Util::FloorToInt(float number) { return static_cast<int>(floorf(number)); }
float Util::InverseLerp(float start, float end, float number) {
  return (number - start) / (end - start);
}
bool Util::IsPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range{
        "Utility::IsPowerOfTwo => Negative numbers are not supported."};
  return !(number == 0) && !(number & (number - 1));
}
float Util::LerpUnclamped(float start, float end, float time) {
  return start * (1 - time) + time * end;
}
float Util::Lerp(float start, float end, float time) {
  time = Clamp01(time);
  return start * (1 - time) + time * end;
}
float Util::Ln(float number) { return logf(number); }
float Util::Log(float number, float base) { return logf(number) / log(base); }
float Util::Log10(float number) { return log10f(number); }
float Util::Max(float a, float b) { return std::max(a, b); }
float Util::Max(std::initializer_list<float> numbers) {
  return std::max(numbers);
}
int Util::Max(std::initializer_list<int> numbers) { return std::max(numbers); }
float Util::Min(float a, float b) { return std::min(a, b); }
float Util::Min(std::initializer_list<float> numbers) {
  return std::min(numbers);
}
int Util::Min(std::initializer_list<int> numbers) { return std::min(numbers); }

float Util::MoveTowards(float current, float target, float maxDelta) {
  return std::min(current + maxDelta, target);
}

int Util::NextPowerOfTwo(int number) {
  if (number < 0)
    throw std::out_of_range{
        "Util::NextPowerOfTwo => Negative numbers are not supported."};
  int ceil = number - 1;
  for (Size i = 1; i < sizeof(ceil) * CHAR_BIT; i *= 2) {
    ceil |= ceil >> i;
  }
  ++ceil;
  return ceil;
}
float Util::Pow(float number, float power) { return powf(number, power); }
float Util::Repeat(float t, float length) {
  return t - floorf(t / length) * length;
}
float Util::Round(float number) { return roundf(number); }
int Util::RoundToInt(float number) { return static_cast<int>(roundf(number)); }
int Util::Sign(float number) { return (number > 0) - (number < 0); }
float Util::Sin(float radian) { return sinf(radian); }
float Util::SmoothStep(float start, float end, float number) {
  number = Util::Clamp01((number - start) / (end - start));
  return Square(number) * (3 - 2 * number);
}
float Util::Sqrt(float number) { return sqrtf(number); }
float Util::Square(float number) { return number * number; }
int Util::Square(int number) { return number * number; }
float Util::Tan(float radian) { return tanf(radian); }
bool Util::FuzzyEquals(float a, float b) { return Abs(a - b) < EPSILON; }
}  // namespace Isetta::Math

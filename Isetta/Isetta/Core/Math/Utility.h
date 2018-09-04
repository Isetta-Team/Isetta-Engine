// "Copyright [2018] Isetta"

#pragma once
#ifndef ISETTA_ISETTA_MATH_UTILITY_H_
#define ISETTA_ISETTA_MATH_UTILITY_H_
#include <initializer_list>

namespace Isetta::Math {
class Utility {
 public:
  static const float PI;
  static const float EPSILON;
  static const float DEG2RAD;
  static const float RAD2DEG;

  inline static float Abs(float number) {
    return number < 0 ? -number : number;
  }

  inline static int Abs(int number) { return number < 0 ? -number : number; }

  static float Acos(float number);
  static float Asin(float number);
  static float Atan(float number);
  static float Atan2(float y, float x);

  static float Ceil(float number);
  static int CellToInt(float number);
  static float Clamp(float start, float end, float number);
  static int Clamp(int start, int end, int number);
  static float Clamp01(float number);
  static int ClosestPowerOfTwo(int number);
  static float Cos(float radian);
  static float Exp(float power);
  static float Floor(float number);
  static int FloorToInt(float number);
  static float InverseLerp(float start, float end, float number);
  static bool IsPowerOfTwo(int number);
  static float Lerp(float start, float end, float time);
  static float Ln(float number);
  static float Log(float number, float base);
  static float Log10(float number);
  static float Max(std::initializer_list<float> numbers);
  static int Max(std::initializer_list<int> numbers);
  static float Min(std::initializer_list<float> numbers);
  static int Min(std::initializer_list<int> numbers);
  static float MoveTowards(float current, float target, float maxDelta);
  static int NextPowerOfTwo(int number);
  static float Pow(float number, float power);
  static float Repeat(float t, float length);
  static float Round(float number);
  static int RoundToInt(float number);
  static int Sign(float number);
  static float Sin(float radian);
  static float SmoothStep(float start, float end, float number);
  static float Sqrt(float number);
  static float Square(float number);
  static int Square(int number);
  static float Tan(float radian);
};
}  // namespace Math

#endif
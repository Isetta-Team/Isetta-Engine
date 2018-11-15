/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <initializer_list>
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API Util {
 public:
  static const float PI;
  static const float PI_HALF;
  static const float EPSILON;
  static const float DEG2RAD;
  static const float RAD2DEG;

  /**
   * \brief Return the abs number of the input parameter
   * \param number The number
   */
  static float Abs(float number);
  /**
   * \brief Return the abs number of the input parameter
   * \param number The integer number
   */
  static int Abs(int number);
  /**
   * \brief Return the acos angle in radian
   * \param number The parameter number
   */
  static float Acos(float number);
  /**
   * \brief Return the asin angle in radian
   * \param number The parameter number
   */
  static float Asin(float number);
  /**
   * \brief Return the atan angle in radian
   * \param number The parameter number
   */
  static float Atan(float number);
  /**
   * \brief Return the atan angle in radian by y/x
   * \param y The y parameter
   * \param x The x parameter
   */
  static float Atan2(float y, float x);
  /**
   * \brief Return the ceiling value of the input parameter
   * \param number The parameter number
   */
  static float Ceil(float number);
  /**
   * \brief Return the integer ceiling value of the input parameter
   * \param number The parameter number
   */
  static int CeilToInt(float number);
  /**
   * \brief Return the clamped number
   * \param start The start value
   * \param end The ending value
   * \param number The number to be clamped
   */
  static float Clamp(float start, float end, float number);
  /**
   * \brief Return the clamped number
   * \param start The start value
   * \param end The ending value
   * \param number The number to be clamped
   */
  static int Clamp(int start, int end, int number);
  /**
   * \brief Clamp the input parameter into range [0, 1]
   * \param number The number to be clamped
   */
  static float Clamp01(float number);
  /**
   * \brief Find the closest power of two number to input parameter
   * \param number The parameter number
   */
  static int ClosestPowerOfTwo(int number);
  /**
   * \brief Return cosine value
   * \param radian The input radian
   */
  static float Cos(float radian);
  /**
   * \brief Return the value e^power
   * \param power The power number
   */
  static float Exp(float power);
  /**
   * \brief Return the floor value of the input parameter
   * \param number The input number
   */
  static float Floor(float number);
  /**
   * \brief Return the integer floor value of the input parameter
   * \param number The input number
   */
  static int FloorToInt(float number);
  /**
   * \brief Calculate the t value of the lerp function
   * \param start The starting number
   * \param end The ending number
   * \param number The number need to be calculated
   */
  static float InverseLerp(float start, float end, float number);
  /**
   * \brief Check if the number is power of two
   * \param number The input number
   */
  static bool IsPowerOfTwo(int number);
  static float LerpUnclamped(float start, float end, float time);
  /**
   * \brief Lerp the value between start and end by time
   * \param start The starting number
   * \param end The ending number
   * \param time The time in lerp function
   */
  static float Lerp(float start, float end, float time);
  /**
   * \brief Return ln(number)
   * \param number The number parameter
   */
  static float Ln(float number);
  /**
   * \brief Return log_base(number)
   * \param number The number parameter
   * \param base The log base
   */
  static float Log(float number, float base);
  /**
   * \brief Return log(number)
   * \param number The number parameter
   */
  static float Log10(float number);
  static float Max(float a, float b);
  /**
   * \brief Return the maximum number from input numbers
   * \param numbers The candidate numbers
   */
  static float Max(std::initializer_list<float> numbers);
  /**
   * \brief Return the maximum number from input numbers
   * \param numbers The candidate numbers
   */
  static int Max(std::initializer_list<int> numbers);
  static float Min(float a, float b);
  /**
   * \brief Return the minimum number from input numbers
   * \param numbers The candidate numbers
   */
  static float Min(std::initializer_list<float> numbers);
  /**
   * \brief Return the minimum number from input numbers
   * \param numbers The candidate numbers
   */
  static int Min(std::initializer_list<int> numbers);
  /**
   * \brief Moves a value current towards target
   * \param current Current value
   * \param target Target value
   * \param maxDelta Max delta value to move
   */
  static float MoveTowards(float current, float target, float maxDelta);
  /**
   * \brief Return the next power of two value
   * \param number The input number
   */
  static int NextPowerOfTwo(int number);
  /**
   * \brief Return f raised to power p
   * \param number The base number
   * \param power The power number
   */
  static float Pow(float number, float power);
  /**
   * \brief Loops the value t, so that it is never larger than length and never
   * smaller than 0
   * \param t The time t
   * \param length The repeat length
   */
  static float Repeat(float t, float length);
  /**
   * \brief Return number rounded to the nearest integer
   * \param number The input number
   */
  static float Round(float number);
  /**
   * \brief Return number rounded to the nearest integer.
   * \param number The input number
   */
  static int RoundToInt(float number);
  /**
   * \brief Return the sign of number
   * \param number The input number
   */
  static int Sign(float number);
  /**
   * \brief Return the sine of angle
   * \param radian The input radian
   */
  static float Sin(float radian);
  /**
   * \brief Interpolates between min and max with smoothing at the limits
   * \param start The starting number
   * \param end The ending number
   * \param number The step time
   */
  static float SmoothStep(float start, float end, float number);
  /**
   * \brief Return square root of number
   * \param number The input number
   */
  static float Sqrt(float number);
  /**
   * \brief Return number^2
   * \param number The input number
   */
  static float Square(float number);
  /**
   * \brief Return number^2
   * \param number The input number
   */
  static int Square(int number);
  /**
   * \brief Returns the tangent of angle in radians
   * \param radian The input radian
   */
  static float Tan(float radian);

  static bool FuzzyEquals(float, float);
};
}  // namespace Isetta::Math

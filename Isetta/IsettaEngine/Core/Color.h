/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Math/Util.h"

namespace Isetta::Math {
class Vector4;
}

namespace Isetta {
class ISETTA_API Color {
 public:
  /// RGB color black (0,0,0,1)
  static const Color black;
  /// RGB color blue (0,0,1,1)
  static const Color blue;
  static const Color brown;
  /// RGB color clear (0,0,0,0)
  static const Color clear;
  /// RGB color cyan (0,1,1,1)
  static const Color cyan;
  /// RGB color green (0,1,0,1)
  static const Color green;
  /// RGB color grey (0.5,0.5,0.5,1)
  static const Color grey;
  static const Color lightGrey;
  /// RGB color magenta (1,0,1,1)
  static const Color magenta;
  static const Color orange;
  /// RGB color red (1,0,0,1)
  static const Color red;
  /// RGB color white (1,1,1,1)
  static const Color white;
  /// RGB color yellow (1,0.92,0.016,1)
  static const Color yellow;

  union {
    struct {
      /// red
      float r;
      /// green
      float g;
      /// blue
      float b;
      /// alpha
      float a;
    };
    float rgba[4];
  };

  // Constructors
  /**
   * @brief Construct a new Color object, deafults to clear
   *
   */
  Color() : r{0}, g{0}, b{0}, a{0} {}
  /**
   * @brief Construct a new Color object
   *
   * @param r red
   * @param g green
   * @param b blue
   * @param a alpha
   */
  Color(float r, float g, float b, float a) : r{r}, g{g}, b{b}, a{a} {}
  Color(float r, float g, float b) : r{r}, g{g}, b{b}, a{1} {}

  /**
   * @brief Construct a new Color object
   *
   * @param v Vector4
   */
  explicit Color(const Math::Vector4& v);

  // Move and Copy Constructors
  /**
   * @brief Copy constructor
   *
   * @param c
   */
  Color(const Color& c) : r{c.r}, g{c.g}, b{c.b}, a{c.a} {}
  /**
   * @brief Move constructor
   *
   * @param c
   */
  Color(Color&& c) : r{c.r}, g{c.g}, b{c.b}, a{c.a} {}
  /**
   * @brief Copy constructor
   *
   * @param c
   * @return Color&
   */
  inline Color& operator=(const Color& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }
  /**
   * @brief Move constructor
   *
   * @param c
   * @return Color&
   */
  inline Color& operator=(Color&& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }

  // Operators
  /**
   * @brief Returns color component rgba from index
   *
   * @param i
   * @return float
   */
  float operator[](int i) const;
  inline bool operator==(const Color& rhs) const {
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
  }
  inline bool operator!=(const Color& rhs) const { return !(*this == rhs); }
  /**
   * @brief Addition, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator+(const Color& rhs) const {
    return Color(Math::Util::Clamp01(r + rhs.r), Math::Util::Clamp01(g + rhs.g),
                 Math::Util::Clamp01(b + rhs.b),
                 Math::Util::Clamp01(a + rhs.a));
  }
  /**
   * @brief Subtraction, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator-(const Color& rhs) const {
    return Color(Math::Util::Clamp01(r - rhs.r), Math::Util::Clamp01(g - rhs.g),
                 Math::Util::Clamp01(b - rhs.b),
                 Math::Util::Clamp01(a - rhs.a));
  }
  /**
   * @brief Multiplication, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator*(const Color& rhs) const {
    return Color(Math::Util::Clamp01(r * rhs.r), Math::Util::Clamp01(g * rhs.g),
                 Math::Util::Clamp01(b * rhs.b),
                 Math::Util::Clamp01(a * rhs.a));
  }
  /**
   * @brief Multiplication, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator*(const float rhs) const {
    return Color(Math::Util::Clamp01(r * rhs), Math::Util::Clamp01(g * rhs),
                 Math::Util::Clamp01(b * rhs), Math::Util::Clamp01(a * rhs));
  }
  /**
   * @brief Multiplication, clamps rgba component from 0 to 1
   *
   * @param lhs
   * @param rhs
   * @return Color
   */
  friend inline Color operator*(const float lhs, Color rhs) {
    return rhs * lhs;
  }
  /**
   * @brief Multiplication, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator*(const int rhs) const {
    return Color(Math::Util::Clamp01(r * rhs), Math::Util::Clamp01(g * rhs),
                 Math::Util::Clamp01(b * rhs), Math::Util::Clamp01(a * rhs));
  }
  /**
   * @brief Multiplication, clamps rgba component from 0 to 1
   *
   * @param lhs
   * @param rhs
   * @return Color
   */
  friend inline Color operator*(const int lhs, Color rhs) { return rhs * lhs; }
  /**
   * @brief Division, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator/(const float rhs) const {
    return Color(Math::Util::Clamp01(r / rhs), Math::Util::Clamp01(g / rhs),
                 Math::Util::Clamp01(b / rhs), Math::Util::Clamp01(a / rhs));
  }
  /**
   * @brief Division, clamps rgba component from 0 to 1
   *
   * @param rhs
   * @return Color
   */
  inline Color operator/(const int rhs) const {
    return Color(Math::Util::Clamp01(r / rhs), Math::Util::Clamp01(g / rhs),
                 Math::Util::Clamp01(b / rhs), Math::Util::Clamp01(a / rhs));
  }
  friend std::istream& operator>>(std::istream& in, Color& color);

  // Conversions
  /**
   * @brief Convert Vector4 to Color
   *
   * @param vec4
   */
  explicit operator Math::Vector4();

  // Functions
  /**
   * @brief Get the grayscale value of the Color (average the components), keep
   * alpha
   *
   * @return float the averaged value
   */
  float GreyScale() const;
  /**
   * @brief Get the maximum color component of rgb
   *
   * @return float
   */
  float MaxColorComponent() const;
  /**
   * @brief Convert the Color to string (including '(,,,)')
   *
   * @return float
   */
  std::string ToString() const;
  /**
   * @brief Lerp the Color from the start to end parameter based on t value,
   * returns the color
   *
   * @param start color to interoplate from
   * @param end color to interoplate towards
   * @param t how far from start to end, new color should be
   * @return Color
   */
  static Color Lerp(Color start, Color end, float t);

  // TODO(Jacob)
  // HSV2RGB
  // RGB2HSV

 private:
};
}  // namespace Isetta

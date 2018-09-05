/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Math/Utility.h"

namespace Isetta::Math {
class Vector4;
}

namespace Isetta {
class Color {
 public:
  // Constants
  static const Color black;
  static const Color blue;
  static const Color clear;
  static const Color cyan;
  static const Color grey;
  static const Color magenta;
  static const Color red;
  static const Color white;
  static const Color yellow;

  float r, g, b, a;

  // Construction

  Color() : r{0}, g{0}, b{0}, a{0} {}
  Color(float r, float g, float b, float a) : r{r}, g{g}, b{b}, a{a} {}

  // Copy and move constructions

  Color(const Color& c) : r{c.r}, g{c.g}, b{c.b}, a{c.a} {}
  Color(Color&& c) : r{c.r}, g{c.g}, b{c.b}, a{c.a} {}
  inline Color& operator=(const Color& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }
  inline Color& operator=(Color&& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }

  // Operators

  float operator[](int i) const;
  inline bool operator==(const Color& rhs) const {
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
  }
  inline bool operator!=(const Color& rhs) const { return !(*this == rhs); }
  inline Color operator+(const Color& rhs) const {
    Math::Utility::Clamp01(0.5f);
    return Color(
        Math::Utility::Clamp01(r + rhs.r), Math::Utility::Clamp01(g + rhs.g),
        Math::Utility::Clamp01(b + rhs.b), Math::Utility::Clamp01(a + rhs.a));
  }
  inline Color operator-(const Color& rhs) const {
    return Color(
        Math::Utility::Clamp01(r - rhs.r), Math::Utility::Clamp01(g - rhs.g),
        Math::Utility::Clamp01(b - rhs.b), Math::Utility::Clamp01(a - rhs.a));
  }
  inline Color operator*(const Color& rhs) const {
    return Color(
        Math::Utility::Clamp01(r * rhs.r), Math::Utility::Clamp01(g * rhs.g),
        Math::Utility::Clamp01(b * rhs.b), Math::Utility::Clamp01(a * rhs.a));
  }
  inline Color operator*(const float rhs) const {
    return Color(
        Math::Utility::Clamp01(r * rhs), Math::Utility::Clamp01(g * rhs),
        Math::Utility::Clamp01(b * rhs), Math::Utility::Clamp01(a * rhs));
  }
  friend inline Color operator*(const float lhs, Color rhs) {
    return rhs * lhs;
  }
  inline Color operator*(const int rhs) const {
    return Color(
        Math::Utility::Clamp01(r * rhs), Math::Utility::Clamp01(g * rhs),
        Math::Utility::Clamp01(b * rhs), Math::Utility::Clamp01(a * rhs));
  }
  friend inline Color operator*(const int lhs, Color rhs) { return rhs * lhs; }
  inline Color operator/(const float rhs) const {
    return Color(
        Math::Utility::Clamp01(r / rhs), Math::Utility::Clamp01(g / rhs),
        Math::Utility::Clamp01(b / rhs), Math::Utility::Clamp01(a / rhs));
  }
  inline Color operator/(const int rhs) const {
    return Color(
        Math::Utility::Clamp01(r / rhs), Math::Utility::Clamp01(g / rhs),
        Math::Utility::Clamp01(b / rhs), Math::Utility::Clamp01(a / rhs));
  }

  // Conversions

  explicit Color(const Math::Vector4& v);

  // Functions

  Color GreyScale() const;
  float MaxColorComponent() const;
  std::string ToString() const;
  static Color Lerp(Color a, Color b, float t);

 private:
};
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Color.h"

#include <stdio.h>
#include <stdexcept>
#include "Core/Math/Vector4.h"

namespace Isetta {
const Color Color::black = Color(0, 0, 0, 1);
const Color Color::blue = Color(0, 0, 1, 1);
const Color Color::clear = Color(0, 0, 0, 0);
const Color Color::cyan = Color(0, 1, 1, 1);
const Color Color::green = Color(0, 1, 0, 1);
const Color Color::grey = Color(0.5f, 0.5f, 0.5f, 1);
const Color Color::magenta = Color(1, 0, 1, 1);
const Color Color::red = Color(1, 0, 0, 1);
const Color Color::white = Color(1, 1, 1, 1);
const Color Color::yellow = Color(1, 0.92f, 0.016f, 1);

float Color::operator[](int i) const {
  switch (i) {
    case 0:
      return r;
    case 1:
      return g;
    case 2:
      return b;
    case 3:
      return a;
    default:
      throw std::out_of_range("Color::[] access out of range");
  }
}

Color::Color(const Math::Vector4& v) : r{v.x}, g{v.y}, b{v.z}, a{v.w} {}

Color::operator Math::Vector4() { return Math::Vector4(r, g, b, a); }

float Color::GreyScale() const { return (r + g + b) / 3.0f; }

float Color::MaxColorComponent() const { return Math::Util::Max({r, g, b}); }

std::string Color::ToString() const {
  char buffer[26];
  sprintf_s(buffer, "Color(%1.1f, %1.1f, %1.1f, %1.1f)", r, g, b, a);
  return buffer;
}

Color Color::Lerp(Color start, Color end, float time) {
  return start * (1.0f - time) + end * time;
}

}  // namespace Isetta

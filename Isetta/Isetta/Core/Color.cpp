/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Color.h"

#include <stdexcept>
#include "Core/Math/Vector4.h"

namespace Isetta {
const Color black = Color(0, 0, 0, 1);
const Color blue = Color(0, 0, 1, 1);
const Color clear = Color(0, 0, 0, 0), cyan = Color(0, 1, 1, 1);
const Color grey = Color(0.5f, 0.5f, 0.5f, 1);
const Color magenta = Color(1, 0, 1, 1), red = Color(1, 0, 0, 1);
const Color white = Color(1, 1, 1, 1);
const Color yellow = Color(1, 0.92f, 0.016f, 1);

Color::Color(const Math::Vector4& v) : r{v.x}, g{v.y}, b{v.z}, a{v.w} {}

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
};

Color Color::GreyScale() const {
  float avg = (r + g + b) / 3.0f;
  return Color(avg, avg, avg, a);
}

float Color::MaxColorComponent() const { return Math::Utility::Max({r, g, b}); }

std::string Color::ToString() const {
  return "Color(" + std::to_string(r) + ", " + std::to_string(g) + ", " +
         std::to_string(b) + ", " + std::to_string(a) + ")";
}

Color Color::Lerp(Color start, Color end, float time) {
  return start * (1.0f - time) + end * time;
}

}  // namespace Isetta
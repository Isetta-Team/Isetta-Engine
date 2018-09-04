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
const Color yellow = Color(1, 0.92f, 0.016, 1);

Color(const Vector4& v) : r{v.x}, g{v.y}, b{v.z}, a{v.w} {}

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
      throw std::logic_error;
  }
};

float Color::GreyScale() const float MaxColorComponent() const {
  return std::max(r, std::max(g, b));
}

std::string Color::ToString() const {
  return "Color(" + r + ", " + g + ", " + b + ", " + a + ")";
}

Color Color::Lerp(Color start, Color end, float time) const {
  return start * (1.0f - time) + end * time;
}

}  // namespace Isetta
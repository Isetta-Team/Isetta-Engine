// "Copyright [2018] Isetta"
#pragma once
#ifndef ISETTA_ISETTA_MATH_RECT_H_
#define ISETTA_ISETTA_MATH_RECT_H_
namespace Isetta::Math {
class Rect {
 public:
  float x, y, width, height;

  Rect() : x{0}, y{0}, width{0}, height{0} {}
  Rect(float inX, float inY, float inWidth, float inHeight)
      : x{inX}, y{inY}, width{inWidth}, height{inHeight} {}
  Rect(const class Vector2& position, const class Vector2& size);

  Rect(const Rect& inRect)
      : x{inRect.x}, y{inRect.y}, width{inRect.width}, height{inRect.height} {}
  Rect(Rect&& inRect)
      : x{inRect.x}, y{inRect.y}, width{inRect.width}, height{inRect.height} {}
  inline Rect& operator=(const Rect& inRect) {
    x = inRect.x;
    y = inRect.y;
    width = inRect.width;
    height = inRect.height;
    return *this;
  }
  inline Rect& operator=(Rect&& inRect) {
    x = inRect.x;
    y = inRect.y;
    width = inRect.width;
    height = inRect.height;
    return *this;
  }

  ~Rect() {}

  inline bool operator==(const Rect& rhs) const {
    return x == rhs.x && y == rhs.y && width == rhs.width &&
           height == rhs.height;
  }
  inline bool operator!=(const Rect& rhs) const { return !(*this == rhs); }

  inline void Set(float inX, float inY, float inWidth, float inHeight) {
    x = inX;
    y = inY;
    width = inWidth;
    height = inHeight;
  }

  class Vector2 Position() const;
  class Vector2 Center() const;
  class Vector2 Min() const;
  class Vector2 Max() const;
  inline float Size() const { return height * width; }
  inline float XMin() const { return x; }
  inline float XMax() const { return x + width; }
  inline float YMin() const { return y; }
  inline float YMax() const { return y + height; }

  bool Contains(const class Vector2& point) const;
  bool Overlaps(const Rect& rhs) const;

  static class Vector2 NormalizedToPoint(const Rect& rect,
                                         const class Vector2& normalizedCoord);
  static class Vector2 PointToNormalized(const Rect& rect,
                                         const class Vector2& point);
};
}  // namespace Math
#endif
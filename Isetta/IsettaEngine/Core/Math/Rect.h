// "Copyright [2018] Isetta"
#pragma once
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API_DECLARE Rect {
 public:
  float x, y, width, height;

  /**
   * \brief Create a zero rectangle
   */
  Rect() : x{0}, y{0}, width{0}, height{0} {}
  /**
   * \brief Create a rectangle from left-top point, width and height
   * \param inX The x of left-top point
   * \param inY The y of left-top point
   * \param inWidth The length of the rectangle
   * \param inHeight The height of the rectangle
   */
  Rect(float inX, float inY, float inWidth, float inHeight)
      : x{inX}, y{inY}, width{inWidth}, height{inHeight} {}
  /**
   * \brief Create a rectangle from left-top point and size
   * \param position The position of the left-top point
   * \param size The size of the rectangle
   */
  Rect(const class Vector2& position, const class Vector2& size);

  Rect(const Rect& inRect)
      : x{inRect.x}, y{inRect.y}, width{inRect.width}, height{inRect.height} {}
  Rect(Rect&& inRect) noexcept
      : x{inRect.x}, y{inRect.y}, width{inRect.width}, height{inRect.height} {}
  inline Rect& operator=(const Rect& inRect) {
    x = inRect.x;
    y = inRect.y;
    width = inRect.width;
    height = inRect.height;
    return *this;
  }
  inline Rect& operator=(Rect&& inRect) noexcept {
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

  /**
   * \brief Set the attributes of the rectangle
   * \param inX The x of the left-top point
   * \param inY The y of the left-top point
   * \param inWidth The width of the rectangle
   * \param inHeight The height of the rectangle
   */
  inline void Set(float inX, float inY, float inWidth, float inHeight) {
    x = inX;
    y = inY;
    width = inWidth;
    height = inHeight;
  }

  inline void SetPosition(float inX, float inY) {
    x = inX;
    y = inY;
  }

  inline void SetSize(float inWidth, float inHeight) {
    width = inWidth;
    height = inHeight;
  }

  /**
   * \brief Get the position of the left-top point
   */
  class Vector2 Position() const;
  /**
   * \brief Get the position of the center point
   */
  class Vector2 Center() const;
  /**
   * \brief Get the left-top point of the rectangle
   */
  class Vector2 Min() const;
  /**
   * \brief Get the right-bottom point of the rectangle
   */
  class Vector2 Max() const;
  /**
   * \brief Get the size of the rectangle
   */
  class Vector2 Size() const;
  /**
   * \brief Get the size of the rectangle
   */
  inline float Area() const;
  /**
   * \brief Get the minimum x of the rectangle
   */
  inline float XMin() const { return x; }
  /**
   * \brief Get the maximum x of the rectangle
   */
  inline float XMax() const { return x + width; }
  /**
   * \brief Get the minimum y of the rectangle
   */
  inline float YMin() const { return y; }
  /**
   * \brief Get the maximum y of the rectangle
   */
  inline float YMax() const { return y + height; }

  bool Contains(const class Vector2& point) const;
  bool Overlaps(const Rect& rhs) const;

  static class Vector2 NormalizedToPoint(const Rect& rect,
                                         const class Vector2& normalizedCoord);
  static class Vector2 PointToNormalized(const Rect& rect,
                                         const class Vector2& point);
  static bool FuzzyEqual(const Rect& lhs, const Rect& rhs);
};
}  // namespace Isetta::Math

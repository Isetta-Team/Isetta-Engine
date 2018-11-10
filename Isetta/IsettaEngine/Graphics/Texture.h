/*
 * Copyright (c) 2018 Isetta
 */
#include <string>
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2Int.h"

namespace Isetta {
class Texture {
 private:
  Math::Vector2Int size;
  unsigned int texture;
  std::string_view fileName;

 public:
  Texture(std::string_view fileName) : fileName{fileName} {}
  ~Texture() = default;

  void LoadTexture();

  inline unsigned int GetTexture() const { return texture; }
  inline int GetWidth() const { return size.x; }
  inline int GetHeight() const { return size.y; }
  inline Math::Vector2Int GetSize() const { return size; }
};
}  // namespace Isetta
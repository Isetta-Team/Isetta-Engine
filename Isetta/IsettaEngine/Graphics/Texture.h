/*
 * Copyright (c) 2018 Isetta
 */
#include <string>
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2Int.h"

typedef int H3DRes;

namespace Isetta {
class Texture {
 private:
  std::string_view fileName;
  unsigned int texture = 0;
  Math::Vector2Int size;
  H3DRes h3dres;

 public:
  Texture(std::string_view fileName, bool load = true);
  ~Texture();

  void LoadTexture();
  void UnloadTexture();

  inline unsigned int GetTexture() const { return texture; }
  inline int GetWidth() const { return size.x; }
  inline int GetHeight() const { return size.y; }
  inline Math::Vector2Int GetSize() const { return size; }
};
}  // namespace Isetta
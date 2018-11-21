/*
 * Copyright (c) 2018 Isetta
 */
#include <string>
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2Int.h"
#include "ISETTA_API.h"

typedef int H3DRes;

namespace Isetta {
class ISETTA_API Texture {
 private:
  std::string_view fileName;
  unsigned int texture = 0;
  Math::Vector2Int size;
  H3DRes h3dres;

 public:
  Texture() = default;
  Texture(std::string_view fileName, bool load = true);
  ~Texture();

  Texture(const Texture& tex) : fileName{tex.fileName} {
    if (tex.texture) Load();
  }
  Texture(Texture&& tex) : fileName{tex.fileName} {
    texture = tex.texture;
    size = tex.size;
    tex.texture = 0;
  }
  inline Texture& operator=(const Texture& tex) {
    fileName = tex.fileName;
    if (tex.texture) Load();
    return *this;
  }
  inline Texture& operator=(Texture&& tex) {
    fileName = tex.fileName;
    texture = tex.texture;
    size = tex.size;
    tex.texture = 0;
    return *this;
  }

  void Load();
  void Unload();

  inline unsigned int GetTexture() const { return texture; }
  inline int GetWidth() const { return size.x; }
  inline int GetHeight() const { return size.y; }
  inline Math::Vector2Int GetSize() const { return size; }
};
}  // namespace Isetta
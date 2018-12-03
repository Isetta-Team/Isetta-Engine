/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/Texture.h"

#include <glad/glad.h>
#include <string_view>
#include "Graphics/RenderModule.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "Util.h"

namespace Isetta {
Texture::Texture(std::string_view fileName, bool load) : fileName{fileName} {
  if (load) Load();
}
Texture::~Texture() { Unload(); }
void Texture::Load() {
  if (texture) return;
  h3dres = h3dAddResource(H3DResTypes::Texture, fileName.data(), 0);
  RenderModule::LoadResourceFromDisk(
      h3dres, Util::StrFormat("Texture::LoadResourceFromFile => "
                              "Cannot load the resource from %s",
                              fileName.data()));
  data = static_cast<U8*>(h3dMapResStream(
      h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgPixelStream, true, false));
  size.x =
      h3dGetResParamI(h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgWidthI);
  size.y =
      h3dGetResParamI(h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgHeightI);
  h3dUnmapResStream(h3dres);

  int i = 0;
  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      std::swap(data[i], data[i + 2]);
      i += 4;
    }
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA,
  //             GL_UNSIGNED_BYTE, data);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
}
void Texture::Unload() {
  if (!texture) return;
  h3dRemoveResource(h3dres);
  glDeleteTextures(1, &texture);
  texture = 0;
}
}  // namespace Isetta

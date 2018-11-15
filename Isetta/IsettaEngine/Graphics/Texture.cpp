/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/Texture.h"

#ifndef __  // GLAD must be placed first
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include <Horde3D.h>
#include <string_view>
#include "Graphics/RenderModule.h"
#include "Util.h"

namespace Isetta {
Texture::Texture(std::string_view fileName, bool load) : fileName{fileName} {
  if (load) LoadTexture();
}
Texture::~Texture() { UnloadTexture(); }
void Texture::LoadTexture() {
  if (texture) return;
  h3dres = h3dAddResource(H3DResTypes::Texture, fileName.data(), 0);
  RenderModule::LoadResourceFromDisk(
      h3dres, Util::StrFormat("Texture::LoadResourceFromFile => "
                              "Cannot load the resource from %s",
                              fileName.data()));
  U8* data = static_cast<U8*>(h3dMapResStream(
      h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgPixelStream, true, false));
  size.x =
      h3dGetResParamI(h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgWidthI);
  size.y =
      h3dGetResParamI(h3dres, H3DTexRes::ImageElem, 0, H3DTexRes::ImgHeightI);
  h3dUnmapResStream(h3dres);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, data);
}
void Texture::UnloadTexture() {
  if (!texture) return;
  h3dRemoveResource(h3dres);
  glDeleteTextures(1, &texture);
  texture = 0;
}
}  // namespace Isetta

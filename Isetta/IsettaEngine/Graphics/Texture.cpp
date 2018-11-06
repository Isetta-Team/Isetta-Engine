/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/Texture.h"
#include <Horde3D.h>
#include <string_view>

#pragma once
#ifndef __  // GLAD must be placed first
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Graphics/RenderModule.h"
#include "Util.h"

namespace Isetta {

void Texture::LoadTexture() {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::Texture, fileName.data(), 0);
  RenderModule::LoadResourceFromDisk(
      renderResource, Util::StrFormat("Texture::LoadResourceFromFile => "
                                      "Cannot load the resource from %s",
                                      fileName.data()));
  // TODO(Chaojie) Look for way to free this memory before window close
  U8* data =
      static_cast<U8*>(h3dMapResStream(renderResource, H3DTexRes::ImageElem, 0,
                                       H3DTexRes::ImgPixelStream, true, false));
  size.x = h3dGetResParamI(renderResource, H3DTexRes::ImageElem, 0,
                           H3DTexRes::ImgWidthI);
  size.y = h3dGetResParamI(renderResource, H3DTexRes::ImageElem, 0,
                           H3DTexRes::ImgHeightI);
  h3dUnmapResStream(renderResource);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, data);
}
}  // namespace Isetta

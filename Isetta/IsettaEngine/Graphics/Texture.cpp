/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/Texture.h"
#include <Horde3D.h>
#include "RenderModule.h"
#include "Util.h"

namespace Isetta {

U8* Texture::LoadTexture(const std::string& textureName, int* width,
                         int* height) {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::Texture, textureName.c_str(), 0);
  RenderModule::LoadResourceFromDisk(
      renderResource, Util::StrFormat("Texture::LoadResourceFromFile => "
                                      "Cannot load the resource from %s",
                                      textureName.c_str()));
  // TODO(Chaojie) + TODO(Jacob): With .png it's rgb; with .tga it's rgba(or rgb0)
  // ImgPixelStream  - Pixel data of an image. The data
  // layout matches the layout specified by the texture format with the
  // exception that half-float is converted to float. The first element in the
  // data array corresponds to the lower left corner.
  U8* data =
      static_cast<U8*>(h3dMapResStream(renderResource, H3DTexRes::ImageElem, 0,
                                       H3DTexRes::ImgPixelStream, true, false));
  *width = h3dGetResParamI(renderResource, H3DTexRes::ImageElem, 0,
                           H3DTexRes::ImgWidthI);
  *height = h3dGetResParamI(renderResource, H3DTexRes::ImageElem, 0,
                            H3DTexRes::ImgHeightI);
  h3dUnmapResStream(renderResource);
  return data;
}
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Scene/Component.h"
#include <Horde3D.h>

namespace Isetta {
class MeshComponent : public Component {
  H3DNode renderNode;

  MeshComponent();
  ~MeshComponent();

  void UpdateTransform();

 public:
  void LoadResource(std::string resourceName);

 protected:
  H3DRes LoadResourceFromFile(std::string resourceName) const;

  friend class AnimationNode;
  friend class Entity;
  friend class RenderModule;

  static class RenderModule* renderModule;
};
}  // namespace Isetta

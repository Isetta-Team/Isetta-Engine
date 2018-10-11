/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <string>
#include "Scene/Component.h"

namespace Isetta {
ISETTA_API class MeshComponent : public Component {
  H3DNode renderNode;
  H3DRes renderResource;

  explicit MeshComponent(std::string resourceName);
  ~MeshComponent();

  void UpdateTransform();

 public:
 protected:
  H3DRes LoadResourceFromFile(std::string resourceName) const;

  void OnEnable() override;
  void OnDisable() override;

  friend class AnimationComponent;
  friend class Entity;
  friend class MemoryManager;
  friend class FreeListAllocator;
  friend class RenderModule;

  static class RenderModule* renderModule;
};
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <string>
#include "Scene/Component.h"

namespace Isetta {
class ISETTA_API_DECLARE MeshComponent : public Component {
  H3DNode renderNode{NULL};
  H3DRes renderResource{NULL};

  explicit MeshComponent(const std::string& resourceName);
  ~MeshComponent();

  void UpdateTransform() const;

 protected:
  static H3DRes LoadResourceFromFile(const std::string& resourceName);

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

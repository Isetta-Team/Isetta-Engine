/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Math/Math.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "SID/sid.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(MeshComponent, Component, false)
H3DNode renderNode{0};
H3DRes renderResource{0};

std::unordered_map<StringId, H3DNode> joints;

explicit MeshComponent(std::string_view resourceName);
~MeshComponent();

void UpdateTransform() const;

protected:
static H3DRes LoadResourceFromFile(std::string_view resourceName);

void OnEnable() override;
void OnDisable() override;
void OnDestroy() override;

friend class AnimationComponent;
friend class Entity;
friend class MemoryManager;
friend class FreeListAllocator;
friend class RenderModule;

public:
std::tuple<Math::Vector3, Math::Quaternion> GetJointWorldTransform(
    std::string jointName);

static class RenderModule* renderModule;
DEFINE_COMPONENT_END(MeshComponent, Component)
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <string>
#include <string_view>
#include "Core/Math/Math.h"
#include "SID/sid.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(MeshComponent, Component, false)
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
END_COMPONENT(MeshComponent, Component)
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Horde3D.h>
#include "Scene/Component.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
BEGIN_COMPONENT(ParticleSystemComponent, Component, false)
H3DNode renderNode{0};
Array<H3DNode> emitters;
H3DRes renderResource{0};

explicit ParticleSystemComponent(std::string_view resourceName);
// ~ParticleSystemComponent();

void UpdateTransform() const;
void UpdateEmitter(float deltaTime) const;

protected:
static H3DRes LoadResourceFromFile(std::string_view resourceName);
void Awake() override;
void OnEnable() override;
void OnDisable() override;
void OnDestroy() override;

friend class Entity;
friend class MemoryManager;
friend class FreeListAllocator;
friend class RenderModule;

static inline class RenderModule* renderModule;

END_COMPONENT(ParticleSystemComponent, Component)
}

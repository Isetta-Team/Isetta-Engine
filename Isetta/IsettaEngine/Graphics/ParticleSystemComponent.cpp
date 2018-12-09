/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/ParticleSystemComponent.h"
#include "Core/EngineResource.h"
#include "Graphics/RenderModule.h"
#include "Scene/Transform.h"
#include "Util.h"
#include "brofiler/ProfilerCore/Brofiler.h"

Isetta::ParticleSystemComponent::ParticleSystemComponent() {
  renderResource = LoadResourceFromFile(EngineResource::defaultParticle);
}

Isetta::ParticleSystemComponent::ParticleSystemComponent(
    std::string_view resourceName) {
  renderResource = LoadResourceFromFile(resourceName);
}

void Isetta::ParticleSystemComponent::UpdateTransform() const {
  PROFILE Transform::SetH3DNodeTransform(renderNode, *transform);
}

void Isetta::ParticleSystemComponent::UpdateEmitter(float deltaTime) const {
  for (const auto& emitter : emitters) {
    h3dUpdateEmitter(emitter, deltaTime);
  }
}

H3DRes Isetta::ParticleSystemComponent::LoadResourceFromFile(
    std::string_view resourceName) {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.data(), 0);

  RenderModule::LoadResourceFromDisk(
      renderResource,
      Util::StrFormat("ParticleSystemComponent::LoadResourceFromFile => "
                      "Cannot load the resource from %s",
                      resourceName.data()));

  return renderResource;
}

void Isetta::ParticleSystemComponent::Awake() {
  renderModule->particleSystemComponents.push_back(this);
}

void Isetta::ParticleSystemComponent::OnEnable() {
  if (renderNode == 0) {
    renderNode = h3dAddNodes(H3DRootNode, renderResource);
    unsigned int count = h3dFindNodes(renderNode, "", H3DNodeTypes::Emitter);
    emitters.Reserve(count + 1);
    for (int i = 0; i < count; ++i) {
      emitters.PushBack(h3dGetNodeFindResult(i));
    }
  } else {
    h3dSetNodeFlags(renderNode, 0, true);
  }
}

void Isetta::ParticleSystemComponent::OnDisable() {
  h3dSetNodeFlags(renderNode, H3DNodeFlags::NoDraw, true);
}

void Isetta::ParticleSystemComponent::OnDestroy() {
  renderModule->particleSystemComponents.remove(this);
}

void Isetta::ParticleSystemComponent::StartEmttting() {
  hasStarted = true;
}

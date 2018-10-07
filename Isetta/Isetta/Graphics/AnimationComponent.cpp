/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/AnimationComponent.h"
#include <Horde3D.h>
#include "Core/Config/Config.h"

namespace Isetta {
RenderModule* AnimationComponent::renderModule{nullptr};

AnimationComponent::AnimationComponent(MeshComponent* model)
    : previousState{-1},
      currentState{0},
      animatedModel{model},
      isPlaying{false} {
  ASSERT(renderModule != nullptr);
  renderModule->animationComponents.push_back(this);
}

int AnimationComponent::AddAnimation(std::string animationFilename, int layer,
                                     std::string startNode, bool additive) {
  totalStates++;
  return AddAnimation(animationFilename, layer, startNode, additive,
                      totalStates - 1);
}

int AnimationComponent::AddAnimation(std::string animationFilename, int layer,
                                     std::string startNode, bool additive,
                                     int stateIndex) {
  H3DRes res = LoadResourceFromFile(animationFilename);
  h3dSetupModelAnimStage(animatedModel->renderNode, stateIndex, res, layer,
                         startNode.c_str(), additive);
  return stateIndex;
}

void AnimationComponent::UpdateAnimation(float deltaTime) {
  if (isPlaying) {
    // TODO(Chaojie): Animation frame rate;
    animationTime += deltaTime * 30;
    h3dSetModelAnimParams(animatedModel->renderNode, currentState,
                          animationTime, 1);

    h3dUpdateModel(
        animatedModel->renderNode,
        H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
  }
}

void AnimationComponent::Play() { isPlaying = true; }
void AnimationComponent::Stop() { isPlaying = false; }
void AnimationComponent::OnEnable() {
  previousState = -1;
  currentState = 0;
  animationTime = 0;
  isPlaying = true;
}
void AnimationComponent::OnDisable() {
  isPlaying = false;
}


H3DRes AnimationComponent::LoadResourceFromFile(std::string resourceName) {
  H3DRes res = h3dAddResource(H3DResTypes::Animation, resourceName.c_str(), 0);
  RenderModule::LoadResourceFromDisk(
      res, Util::StrFormat("AnimationComponent::LoadResourceFromFile => Cannot "
                           "load the resource from %s",
                           resourceName.c_str()));
  return res;
}
}  // namespace Isetta

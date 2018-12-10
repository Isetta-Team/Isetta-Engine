/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/AnimationComponent.h"

#include "Core/Config/Config.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "Util.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
RenderModule* AnimationComponent::renderModule{nullptr};

AnimationComponent::AnimationComponent(MeshComponent* model)
    : previousState{-1},
      currentState{0},
      totalStates{0},
      animatedModel{model},
      isPlaying{false},
      animationTime{0},
      prevAnimationTime{0},
      blendWeight{1} {
  ASSERT(renderModule != nullptr);
  renderModule->animationComponents.push_back(this);
}

int AnimationComponent::AddAnimation(std::string_view filename) {
  return AddAnimation(filename, 0, "", false, totalStates++);
}

int AnimationComponent::AddAnimation(std::string_view animationFilename,
                                     int layer, std::string_view startNode,
                                     bool additive) {
  return AddAnimation(animationFilename, layer, startNode, additive,
                      totalStates++);
}

int AnimationComponent::AddAnimation(std::string_view animationFilename,
                                     int layer, std::string_view startNode,
                                     bool additive, int stateIndex) {
  H3DRes res = LoadResourceFromFile(animationFilename);
  h3dSetupModelAnimStage(animatedModel->renderNode, stateIndex, res, layer,
                         startNode.data(), additive);
  h3dSetModelAnimParams(animatedModel->renderNode, stateIndex, 0, 1);
  return stateIndex;
}

void AnimationComponent::UpdateAnimation(float deltaTime) {
  PROFILE
  if (isPlaying) {
    // TODO(Chaojie): Animation frame rate;
    prevAnimationTime += deltaTime * 30;
    animationTime += deltaTime * 30;
    if (blendWeight >= 1) {
      h3dSetModelAnimParams(animatedModel->renderNode, currentState,
                            animationTime, 1);
    } else {
      blendWeight += deltaTime / blendDuration;
      if (blendWeight > 1) blendWeight = 1;
      h3dSetModelAnimParams(animatedModel->renderNode, previousState,
                            prevAnimationTime, 1 - blendWeight);
      h3dSetModelAnimParams(animatedModel->renderNode, currentState,
                            animationTime, blendWeight);
    }

    h3dUpdateModel(
        animatedModel->renderNode,
        H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
  }
}

void AnimationComponent::TransitToAnimationState(int state, float duration) {
  if (state == currentState) return;
  previousState = currentState;
  prevAnimationTime = animationTime;
  animationTime = 0;
  currentState = state;
  blendWeight = 0;
  blendDuration = duration;
}

void AnimationComponent::Play() { isPlaying = true; }
void AnimationComponent::Stop() { isPlaying = false; }
void AnimationComponent::OnEnable() {
  previousState = -1;
  currentState = 0;
  animationTime = 0;
  isPlaying = true;
}
void AnimationComponent::OnDisable() { isPlaying = false; }
void AnimationComponent::OnDestroy() {
  renderModule->animationComponents.remove(this);
}

H3DRes AnimationComponent::LoadResourceFromFile(std::string_view resourceName) {
  H3DRes res = h3dAddResource(H3DResTypes::Animation, resourceName.data(), 0);
  RenderModule::LoadResourceFromDisk(
      res, Util::StrFormat("AnimationComponent::LoadResourceFromFile => Cannot "
                           "load the resource from %s",
                           resourceName.data()));
  return res;
}
}  // namespace Isetta

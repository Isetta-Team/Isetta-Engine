/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/AnimationNode.h"
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include "Core/Config/Config.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"

namespace Isetta {
RenderModule* AnimationNode::renderModule{nullptr};

AnimationNode::AnimationNode(ModelNode* model)
    : previousState{-1},
      currentState{0},
      animatedModel{model},
      isPlaying{false} {
  if (renderModule != nullptr) {
    renderModule->animationNodes.push_back(this);
  }
}

int AnimationNode::AddAnimation(std::string animationFilename, int layer,
                                std::string startNode, bool additive) {
  totalStates++;
  return AddAnimation(animationFilename, layer, startNode, additive,
                      totalStates - 1);
}

int AnimationNode::AddAnimation(std::string animationFilename, int layer,
                                std::string startNode, bool additive,
                                int stateIndex) {
  H3DRes res = LoadResourceFromFile(animationFilename);
  h3dSetupModelAnimStage(animatedModel->renderNode, stateIndex, res, layer,
                         startNode.c_str(), additive);
  return stateIndex;
}

void AnimationNode::UpdateAnimation(float deltaTime) {
  if (isPlaying) {
    h3dSetModelAnimParams(animatedModel->renderNode, currentState,
                          EngineLoop::GetGameClock().GetElapsedTime() * 30, 1);
    h3dUpdateModel(
        animatedModel->renderNode,
        H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);
  }
}

void AnimationNode::Play() { isPlaying = true; }
void AnimationNode::Stop() { isPlaying = false; }

H3DRes AnimationNode::LoadResourceFromFile(std::string resourceName) {
  H3DRes res = h3dAddResource(H3DResTypes::Animation, resourceName.c_str(), 0);
  if (!h3dutLoadResourcesFromDisk(
          Config::Instance().resourcePath.GetVal().c_str())) {
    h3dutDumpMessages();
    throw std::exception(
        std::string(
            "LightNode::LoadResourceFromFile: Cannot load the resource from " +
            resourceName)
            .c_str());
  }
  return res;
}
}  // namespace Isetta

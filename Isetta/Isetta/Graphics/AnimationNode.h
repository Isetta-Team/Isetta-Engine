/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RenderNode.h"
#include "Graphics/ModelNode.h"

namespace Isetta {
class AnimationNode : RenderNode {
 public:
  explicit AnimationNode(ModelNode* model);
  int AddAnimation(std::string animationFilename, int layer,
                    std::string startNode, bool additive);
  int AddAnimation(std::string animationFilename, int layer,
                    std::string startNode, bool additive, int stateIndex);
  void UpdateAnimation(float deltaTime);
  static void Play();
  static void Stop();

 protected:
  H3DRes LoadResourceFromFile(std::string resourceName) override;

private:
  int previousState;
  int currentState;
  int totalStates;
  ModelNode* animatedModel;
  static bool isPlaying;

  static class RenderModule* renderModule;
  friend class RenderModule;
};
}  // namespace Isetta

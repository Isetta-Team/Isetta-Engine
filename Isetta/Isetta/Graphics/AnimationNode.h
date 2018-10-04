/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/MeshComponent.h"
#include "Graphics/RenderNode.h"

namespace Isetta {
class AnimationNode : RenderNode {
 public:
  explicit AnimationNode(MeshComponent* model);
  int AddAnimation(std::string animationFilename, int layer,
                   std::string startNode, bool additive);
  int AddAnimation(std::string animationFilename, int layer,
                   std::string startNode, bool additive, int stateIndex);
  /**
   * \brief Update the animation by delta time
   * \param deltaTime The update deltaTime
   */
  void UpdateAnimation(float deltaTime);
  /**
   * \brief Play the animation
   */
  void Play();
  /**
   * \brief Pause the animation
   */
  void Stop();

 protected:
  H3DRes LoadResourceFromFile(std::string resourceName) override;

 private:
  int previousState;
  int currentState;
  int totalStates;
  MeshComponent* animatedModel;
  bool isPlaying;

  static class RenderModule* renderModule;
  friend class RenderModule;
};
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/MeshComponent.h"
#include "Graphics/RenderNode.h"

namespace Isetta {
class AnimationComponent : public Component {
 public:
  explicit AnimationComponent(MeshComponent* model);
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

  void OnEnable() override;
  void OnDisable() override;

 protected:
  H3DRes LoadResourceFromFile(std::string resourceName);

 private:
  int previousState;
  int currentState;
  int totalStates;
  MeshComponent* animatedModel;
  bool isPlaying;
  // TODO(Chaojie): SubClock?
  float animationTime;

  static class RenderModule* renderModule;
  friend class RenderModule;
};
}  // namespace Isetta

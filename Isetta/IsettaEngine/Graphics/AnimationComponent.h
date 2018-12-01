/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/MeshComponent.h"

namespace Isetta {
/**
 * @brief Component that controls the animation of a mesh
 *
 */
DEFINE_COMPONENT(AnimationComponent, Component, false)
public:
/**
 * @brief Construct a new Animation Component object
 *
 * @param model which holds the rig to animate
 */
explicit AnimationComponent(MeshComponent* model);

/**
 * @brief Add animation to be able to transition/animate
 *
 * @param filename of the animation file (located in resource_path)
 * @param layer of the animation
 * @param startNode node (joint) in skeleton that the animation starts at, set
 * to 0 for root joint
 * @param additive whether the animation is additive to other animations
 * @return int handle of the animation
 */
int AddAnimation(std::string_view filename, int layer,
                 std::string_view startNode, bool additive);
/**
 * @brief Add animation to be able to transition/animate
 *
 * @param filename of the animation file (located in resource_path)
 * @param layer of the animation
 * @param startNode node (joint) in skeleton that the animation starts at, set
 * to 0 for root joint
 * @param additive whether the animation is additive to other animations
 * @param statIndex
 * @return int handle of the animation
 */
int AddAnimation(std::string_view filename, int layer,
                 std::string_view startNode, bool additive, int stateIndex);

/**
 * @brief Update the animation by delta time
 *
 * @param deltaTime The update deltaTime
 */
void UpdateAnimation(float deltaTime);
/**
 * @brief Transition to specified animation state
 *
 * @param state to transition to
 * @param time of the transition
 */
void TransitToAnimationState(int state, float time);

/**
 * @brief Play the animation
 */
void Play();

/**
 * @brief Pause the animation
 */
void Stop();

void OnEnable() override;
void OnDisable() override;
void OnDestroy() override;

protected:
H3DRes LoadResourceFromFile(std::string_view resourceName);

private:
int previousState;
int currentState;
int totalStates;
MeshComponent* animatedModel;
bool isPlaying;
// TODO(Chaojie): SubClock?
float animationTime;
float blendWeight;
float blendDuration;

static class RenderModule* renderModule;
friend class RenderModule;
DEFINE_COMPONENT_END(AnimationComponent, Component)
}  // namespace Isetta

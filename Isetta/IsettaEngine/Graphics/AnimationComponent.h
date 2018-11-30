/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/MeshComponent.h"

namespace Isetta {
DEFINE_COMPONENT(AnimationComponent, Component, false)
public:
explicit AnimationComponent(MeshComponent* model);
int AddAnimation(std::string_view animationFilename, int layer,
                 std::string_view startNode, bool additive);
int AddAnimation(std::string_view animationFilename, int layer,
                 std::string_view startNode, bool additive, int stateIndex);
/**
 * \brief Update the animation by delta time
 * \param deltaTime The update deltaTime
 */
void UpdateAnimation(float deltaTime);
void TransitToAnimationState(int state, float time);
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

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Math/Vector3.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(JointFollow, Component, false)
private:
class MeshComponent* const follow;
std::string jointName;
Math::Vector3 posOffset, rotOffset;

public:
JointFollow() = default;
JointFollow(MeshComponent* follow, const std::string_view jointName)
    : follow{follow}, jointName{jointName} {}
JointFollow(MeshComponent* follow, const std::string_view jointName,
            const Math::Vector3& posOffset, const Math::Vector3& rotOffset)
    : follow{follow},
      jointName{jointName},
      posOffset{posOffset},
      rotOffset{rotOffset} {}

void Update() override;
#ifdef _EDITOR
void GuiUpdate() override;
void Open();
#endif
DEFINE_COMPONENT_END(JointFollow, Component)
}  // namespace Isetta

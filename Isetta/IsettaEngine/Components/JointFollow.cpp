/*
 * Copyright (c) 2018 Isetta
 */
#include "JointFollow.h"

#include "Graphics/MeshComponent.h"
#include "Scene/Transform.h"
#ifdef _EDITOR
#include "Components/Editor/Inspector.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#endif

namespace Isetta {
void JointFollow::Update() {
  if (!follow) return;
  auto [pos, rot] = follow->GetJointWorldTransform(jointName);
  transform->SetWorldPos(pos + posOffset);
  transform->SetWorldRot(rot);
  transform->RotateWorld(rotOffset);
}

#ifdef _EDITOR
void JointFollow::GuiUpdate() {
  const Inspector* const inspector = Inspector::Instance();
  if (!inspector || !inspector->IsSelected(transform)) return;

  static RectTransform rect{Math::Rect{10, 10, 350, 100}, GUI::Pivot::Left};
  GUI::Window(rect, "Joint Follow", [&]() {
    RectTransform local{{10, 10, 0, 0}};
    GUI::InputVector3(local, "Position Offset", &posOffset);
    local.rect.y += 25;
    GUI::InputVector3(local, "Rotation Offset", &rotOffset);
  });
}
#endif
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/Editor/Inspector.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix3.h"
#include "Graphics/GUI.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "imgui/imgui.h"

namespace Isetta {
#ifdef _EDITOR
Inspector *Inspector::instance = nullptr;

Inspector::Inspector(std::string title, const bool isOpen, bool isStatic,
                     Transform *const target)
    : target{target}, title{title}, isOpen{isOpen} {
  if (isStatic) instance = this;
}

void Inspector::GuiUpdate() {
  // Happens when selected entity is destroyed
  if (!target || target->entity == nullptr) {
    target = nullptr;
    GUI::Window(rectTransform, title, [&]() {}, &isOpen);
    return;
  }

  GUI::Window(
      rectTransform, title,
      [&]() {
        std::string parentName =
            target->GetParent() ? target->GetParent()->GetName() : "null";

        float height = 5;
        float padding = 15;
        RectTransform rect =
            RectTransform{Math::Rect{padding, height, 300, 100}};
        GUI::Text(rect, target->entity->GetEntityIdString());
        rect.rect.y += 15;

        GUI::Text(rect, target->entity->GetName());
        rect.rect.y += 15;

        GUI::Text(rect, "World Position " + target->GetWorldPos().ToString());
        rect.rect.y += 20;

        Math::Vector3 tmp = target->GetLocalPos();
        GUI::InputVector3(rect, "Local Position", &tmp);
        target->SetLocalPos(tmp);
        rect.rect.y += 25;

        GUI::Text(rect,
                  "World Rotation " + target->GetWorldEulerAngles().ToString());
        rect.rect.y += 20;

        tmp = target->GetLocalEulerAngles();
        GUI::InputVector3(rect, "Local Rotation", &tmp);
        target->SetLocalRot(tmp);
        rect.rect.y += 25;

        tmp = target->GetLocalScale();
        GUI::InputVector3(rect, "Local Scale", &tmp);
        target->SetLocalScale(tmp);
        rect.rect.y += 25;

        GUI::Text(rect, "Parent: " + target->GetParent()->GetName());
        rect.rect.y += 20;

        if (GUI::Button(RectTransform{Math::Rect{0, rect.rect.y, 300, 30},
                                      GUI::Pivot::Top, GUI::Pivot::Top},
                        "Reset")) {
          target->SetLocalRot(Math::Quaternion::identity);
          target->SetLocalPos(Math::Vector3::zero);
          target->SetLocalScale(Math::Vector3::one);
        }

        ImGui::Separator();

        rect.rect.y += 40;
        GUI::Text(RectTransform{Math::Rect{padding, rect.rect.y, 300, 100}},
                  "Components");

        for (const auto &component :
             target->entity->GetComponents<Component>()) {
          Component &comp = *component;
          rect.rect.y += padding;
          GUI::Text(RectTransform{Math::Rect{padding, rect.rect.y, 300, 100}},
                    typeid(comp).name());
        }
      },
      &isOpen);

  Math::Matrix4 temp{};
  temp.SetTopLeftMatrix3(target->GetLocalRot().GetMatrix3());  // rotation
  temp.SetCol(3, target->GetLocalPos(), 1);

  if (target->GetParent() != nullptr) {
    temp = target->GetParent()->GetLocalToWorldMatrix() * temp;
  }

  DebugDraw::Axis(temp);
  DebugDraw::AxisSphere(temp);
}

void Inspector::Open() { isOpen = true; }

bool Inspector::IsSelected(const Transform *const transform) const {
  return target && transform->entity == target->entity;
}

void Inspector::SetAsInstance(bool isStatic) {
  if (isStatic)
    instance = this;
  else  // TODO(Jacob) Maybe perform a check to see if this Inspector was
        // instance Inspector?
    instance = nullptr;
}

const Inspector *Inspector::Instance() { return instance; }
#endif
}  // namespace Isetta

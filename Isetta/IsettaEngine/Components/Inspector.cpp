/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/Inspector.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"

namespace Isetta {
Inspector::Inspector(std::string title, bool isOpen) {
}

void Inspector::GuiUpdate() {
  GUI::Window(
      rectTrans, title.c_str(),
      [&]() {
        std::string parentName = transform->parent == nullptr ? "null" : transform->parent->GetName();

        float height = 5;
        float padding = 15;
        rect = RectTransform{Math::Rect{padding, height, 300, 100}}
        GUI::Text(rect, transform->entity->GetName());
        rect.rect.y += 15;

        GUI::Text(rect,
            "World Position: " + transform->GetWorldPos().ToString());
        rect.rect.y += 15;

        Math::Vector3 tmp = transform->GetLocalPos();
        GUI::InputVector3(rect, "Local Position", &tmp);
        transform->SetLocalPos(tmp);
        rect.rect.y += 15;

        GUI::Text(rect,
            "World Rotation: " + transform->GetWorldEulerAngles().ToString());
        rect.rect.y += 15;

        tmp = transform->GetLocalEulerAngles();
        GUI::InputVector3(rect, "Local Rotation", &tmp);
        transform->SetLocalRot(tmp);
        rect.rect.y += 15;

        tmp = transform->GetLocalScale();
        GUI::InputVector3(rect, "Local Scale", &tmp);
        transform->SetLocalScale(tmp);
        rect.rect.y += 15;

        GUI::Text(rect, "Parent: " + transform->parent->GetName());
        rect.rect.y += 15;

        if (GUI::Button(RectTransform{Math::Rect{0, rect.rect.y, 300, 30}},
                        "Reset"), GUI::Pivot::Center) {
          SetLocalRot(Math::Quaternion::identity);
          SetLocalPos(Math::Vector3::zero);
          SetLocalScale(Math::Vector3::one);
        }

        ImGui::Separator();


        rect.rect.y += 50;
        GUI::Text(RectTransform{Math::Rect{padding, rect.rect.y, 300, 100}},
                  "Components");

        rect.rect.y += padding;

        for (const auto &component : entity->GetComponents()) {
          Component &comp = *component;
          GUI::Text(RectTransform{Math::Rect{padding, height, 300, 100}},
                    typeid(comp).name());
          height += padding;
        }
      },
      &isOpen);

  Math::Matrix4 temp{};
  temp.SetTopLeftMatrix3(localRot.GetMatrix3());  // rotation
  temp.SetCol(3, localPos, 1);

  if (transform->parent != nullptr) {
    temp = transform->parent->GetLocalToWorldMatrix() * temp;
  }
}
} // namespace Isetta


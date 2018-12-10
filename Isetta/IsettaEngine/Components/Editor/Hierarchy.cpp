/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/Editor/Hierarchy.h"

#include "Components/Editor/Inspector.h"
#include "Graphics/GUI.h"
#include "Input/Input.h"
#include "Input/KeyCode.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "Scene/Transform.h"

namespace Isetta {
#ifdef _EDITOR
Hierarchy::Hierarchy(std::string title, bool isOpen, Inspector* inspector)
    : title{title}, isOpen{isOpen}, inspector{inspector} {}
void Hierarchy::GuiUpdate() {
  GUI::Window(
      rectTransform, title,
      [&]() {
        float buttonHeight = 20;
        float buttonWidth = 140;
        float height = 10;
        float left = 10;
        float padding = 20;
        Transform* target = nullptr;

        std::list<Entity*> entities =
            LevelManager::Instance().loadedLevel->GetEntities();

        static Func<int, Transform*> countLevel = [](Transform* t) -> int {
          int i = 0;
          while (t->GetParent() != nullptr) {
            t = t->GetParent();
            ++i;
          }
          return i;
        };

        Action<Transform*> action = [&](Transform* t) {
          int level = countLevel(t);
          GUI::PushID(t->entity->GetEntityIdString());
          float offset = (level - 1) * padding;
          if (GUI::Button(
                  RectTransform{Math::Rect{left + offset, height,
                                           buttonWidth - offset, buttonHeight}},
                  t->GetName())) {
            target = t;
          }
          GUI::PopID();
          height += 1.25f * buttonHeight;
          for (auto it = t->begin(); it != t->end(); ++it) action(*it);
        };
        for (const auto& entity : entities) {
          if (entity->transform->GetParent() ==
              LevelManager::Instance().loadedLevel->levelRoot->transform)
            action(entity->transform);
        }

        if (inspector) {
          if (GUI::IsWindowFocused() &&
              Input::IsMouseButtonPressed(MouseButton::LEFT) && !target)
            inspector->target = target;
          if (target) {
            inspector->target = target;
            inspector->Open();
          }
        }
      },
      &isOpen, {}, GUI::WindowFlags::HorizontalScrollbar);
}

void Hierarchy::Open() { isOpen = true; }
#endif
}  // namespace Isetta

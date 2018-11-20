/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/Editor/Hierarchy.h"

#include "Components/Editor/Inspector.h"
#include "Graphics/GUI.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "Scene/Transform.h"

namespace Isetta {
Hierarchy::Hierarchy(std::string title, bool isOpen, Inspector* inspector)
    : title{title}, isOpen{isOpen}, inspector{inspector} {}
void Hierarchy::GuiUpdate() {
  GUI::Window(rectTransform, title.c_str(),
              [&]() {
                float buttonHeight = 20;
                float buttonWidth = 125;
                float height = 10;
                float left = 5;
                float padding = 20;
                Transform* target = nullptr;

                std::list<Entity*> entities =
                    LevelManager::Instance().loadedLevel->GetEntities();
                for (const auto& entity : entities) {
                  Func<int, Transform*> countLevel = [](Transform* t) -> int {
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
                    if (GUI::Button(RectTransform{Math::Rect{
                                        left + (level - 1) * padding, height,
                                        buttonWidth, buttonHeight}},
                                    t->GetName())) {
                      target = t;
                    }
                    GUI::PopID();
                    height += 1.25f * buttonHeight;
                  };
                  action(entity->transform);
                }

                if (inspector && target) {
                  inspector->target = target;
                  inspector->Open();
                }
              },
              &isOpen, {}, GUI::WindowFlags::HorizontalScrollbar);
}

void Hierarchy::Open() { isOpen = true; }
}  // namespace Isetta

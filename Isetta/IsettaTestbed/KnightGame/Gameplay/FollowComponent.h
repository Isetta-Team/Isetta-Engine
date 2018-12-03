/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace Isetta {
class Entity;
}  // namespace Isetta

namespace KnightGame {
DEFINE_COMPONENT(FollowComponent, Component, true)
private:
Entity* const follow;
bool direction[3];

public:
FollowComponent(Entity* follow, bool direction[3]) : follow{follow} {
  for (int i = 0; i < 3; i++) this->direction[i] = direction[i];
}
void LateUpdate() override;
};  // namespace KnightGame
}  // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::FollowComponent, Component,
                               true>::FollowComponentRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::FollowComponent)),
        std::type_index(typeid(Component)), true);
/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

namespace Isetta {
class MeshComponent;
}

namespace KnightGame {
BEGIN_COMPONENT(SwordController, Component, true)
private:
MeshComponent* knight;

public:
SwordController(MeshComponent* knight) : knight{knight} {}

void Update() override;
};  // namespace KnightGame
}  // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::SwordController, Component,
                               true>::SwordControllerRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::SwordController)),
        std::type_index(typeid(Component)), true);
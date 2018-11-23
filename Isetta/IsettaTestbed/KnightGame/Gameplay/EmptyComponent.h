/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

namespace KnightGame
{
BEGIN_COMPONENT(EmptyComponent, Component, true)
private:
public:
}; // namespace KnightGame
} // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::EmptyComponent, Component,
                               true>::EmptyComponentRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::EmptyComponent)),
        std::type_index(typeid(Component)), true);
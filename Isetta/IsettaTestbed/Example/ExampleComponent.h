/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
/**
 * @brief Component demoing what a component should look like
 *  param1: Name of new component
 *  param2: Name of base/parent component
 *  param3: Whether the component is unique on an entity (ie. [AllowMultiple=false])
 */
namespace Isetta {
DEFINE_COMPONENT(ExampleComponent, Component, true)
public:
void Update() override;
DEFINE_COMPONENT_END(ExampleComponent, Component)
}  // namespace Isetta

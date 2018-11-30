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
BEGIN_COMPONENT(ExampleComponent, Component, true)
public:
void Update() override;
END_COMPONENT(ExampleComponent, Component)
}  // namespace Isetta

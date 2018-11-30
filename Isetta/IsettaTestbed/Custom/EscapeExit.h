/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
namespace Isetta {
BEGIN_COMPONENT(EscapeExit, Component, true)
private:
U64 handle;

public:
void OnEnable() override;
void OnDisable() override;
END_COMPONENT(EscapeExit, Component)
}  // namespace Isetta
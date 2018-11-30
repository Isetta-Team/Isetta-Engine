/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
using namespace Isetta;

/**
 * @brief Demoing of some input capabilities
 *
 */
DEFINE_COMPONENT(InputTestComponent, Component, false)
private:
U64 handleA, handleB, handleC;

public:
void OnEnable() override;
void OnDisable() override;
void Update() override;
DEFINE_COMPONENT_END(InputTestComponent, Component)

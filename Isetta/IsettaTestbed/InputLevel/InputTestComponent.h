/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Core/IsettaAlias.h"

using namespace Isetta;

/**
 * @brief Demoing of some input capabilities
 *
 */
BEGIN_COMPONENT(InputTestComponent, Component, false)
private:
U64 handleA, handleB, handleC;

public:
void OnEnable() override;
void OnDisable() override;
void Update() override;
END_COMPONENT(InputTestComponent, Component)

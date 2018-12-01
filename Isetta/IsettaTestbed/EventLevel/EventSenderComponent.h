/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Construct a new define component object
 *
 */
DEFINE_COMPONENT(EventSenderComponent, Isetta::Component, false)
Isetta::U64 handleA{0}, handleB{0}, handleC{0};
void Start() override;
DEFINE_COMPONENT_END(EventSenderComponent, Isetta::Component)

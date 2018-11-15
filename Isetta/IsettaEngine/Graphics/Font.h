/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "imgui/imgui.h"

struct ISETTA_API Font : private ImFont {
  friend class GUI;
};
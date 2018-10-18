/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

class GUIComponent : public Component {
 private:
  static bool checkbox;

 public:
  void GuiUpdate() override;
};
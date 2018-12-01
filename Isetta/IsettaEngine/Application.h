/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "ISETTA_API.h"

namespace Isetta {

class ISETTA_API Application {
 public:
  /**
   * \brief Start the game. Should be called in your main.cpp
   */
  static void Start();
  /**
   * \brief Exit the game, can be called anywhere
   */
  static void Exit();
};

}  // namespace Isetta

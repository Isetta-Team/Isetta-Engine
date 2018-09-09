/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <sstream>
#include <string>
#include "Core/Debug/Assert.h"

class Convert {
 public:
  template <typename T>
  static inline T StringToT(const std::string& str) {
    std::istringstream istr(str);
    T ret;
    // TODO is this an assert? (probably not)
    istr >> ret;
    return ret;
  }
};
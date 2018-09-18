/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>

namespace Isetta {
template <typename T>
class IStringConvertible {
 public:
  IStringConvertible() = default;

  virtual std::string ToString(const T& val) = 0;
  virtual T FromString(const std::string& str) = 0;
};
}  // namespace Isetta

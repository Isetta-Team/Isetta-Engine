/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include <unordered_map>
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {
class ICVar {
 public:
  const std::string name;
  const StringId nameID;

  virtual void SetVal(const std::string& strVal) = 0;
  // virtual StringId GetType() const { return typeID; }
  template <typename T>
  inline ICVar* Instantiate() {
    return new T;
  }

 protected:
  // const StringId typeID;
  explicit ICVar(std::string name) : name{name}, nameID{SID(name.c_str())} {}
  virtual ~ICVar() {}
};
}  // namespace Isetta

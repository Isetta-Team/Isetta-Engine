/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <sstream>
#include <string>
#include "Core/Config/ICVar.h"
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {

template <typename T>
class CVar : public ICVar {
 public:
  T val;

  CVar(const std::string& name, const T& value) : ICVar(name), val{value} {
    CVarRegistry::RegisterVariable(this);
  }
  explicit CVar(const std::string& name) : ICVar(name), val{T()} {
    CVarRegistry::RegisterVariable(this);
  }

  inline void SetVal(const std::string& strVal) override {
    std::istringstream iss(strVal);
    iss >> val;
  }

  inline T GetVal() const { return val; }
};

class CVarString : public ICVar {
 public:
  std::string sVal;

  CVarString(const std::string& name, const std::string& value);
  explicit CVarString(const std::string& name);

  inline void SetVal(const std::string& strVal) override { sVal = strVal; }

  inline std::string GetVal() const { return sVal; }
};

class CVarVector3 : public ICVar {
 public:
  Math::Vector3 v3Val;

  CVarVector3(const std::string& name, const Math::Vector3& value);
  explicit CVarVector3(const std::string& name);

  inline void SetVal(const std::string& strVal) override {
    v3Val = Math::Vector3::FromString(strVal);
  }

  inline Math::Vector3 GetVal() const { return v3Val; }
};

}  // namespace Isetta

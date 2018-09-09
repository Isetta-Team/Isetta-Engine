/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Config/Convert.h"
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {

#define CVAR_INT 1
#define CVAR_FLOAT 2
#define CVAR_STRING 3
#define CVAR_VECTOR3 4

class ICVar {
 public:
  std::string name;
  StringId sid;

  explicit ICVar(std::string name) : name{name}, sid{SID(name.c_str())} {}
  virtual ~ICVar() {}

  virtual void SetVal(std::string strVal) = 0;
  virtual int GetType() const = 0;

  // virtual inline int GetIVal() const { return int(); }
  // virtual inline float GetFVal() const { return float(); }
  // virtual inline std::string GetStrVal() const { return std::string(); }
  // virtual inline Math::Vector3 GetV3Val() const { return Math::Vector3(); }
};

class CVarInt : public ICVar {
 public:
  int iVal;

  CVarInt(std::string name, int value);

  inline void SetVal(std::string strVal) override {
    iVal = Convert::StringToT<int>(strVal);
  }

  inline int GetIVal() const { return iVal; }
  inline float GetFVal() const { return static_cast<float>(iVal); }

  int GetType() const override { return CVAR_INT; }
};

class CVarFloat : public ICVar {
 public:
  float fVal;

  CVarFloat(std::string name, float value);

  inline void SetVal(std::string strVal) override {
    fVal = Convert::StringToT<float>(strVal);
  }

  inline int GetIVal() const { return fVal; }
  inline float GetFVal() const { return fVal; };

  inline int GetType() const override { return CVAR_FLOAT; }
};

class CVarString : public ICVar {
 public:
  std::string sVal;

  CVarString(std::string name, std::string value);

  inline void SetVal(std::string strVal) override { sVal = strVal; }

  inline std::string GetStrVal() const { return sVal; }

  inline int GetType() const override { return CVAR_STRING; }
};

class CVarVector3 : public ICVar {
 public:
  Math::Vector3 v3Val;

  CVarVector3(std::string name, Math::Vector3 value);

  inline void SetVal(std::string strVal) override {
    v3Val = Convert::StringToT<Math::Vector3>(strVal);
  }

  inline Math::Vector3 GetV3Val() const { return v3Val; }

  inline int GetType() const override { return CVAR_VECTOR3; }
};

}  // namespace Isetta

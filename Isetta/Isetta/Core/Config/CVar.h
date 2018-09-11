/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {

#define CVAR_INT 1
#define CVAR_FLOAT 2
#define CVAR_STRING 3
#define CVAR_VECTOR3 4

class ICVar {
 public:
  const std::string name;
  const StringId sid;

  virtual void SetVal(std::string strVal) = 0;
  virtual int GetType() const = 0;

 protected:
  explicit ICVar(std::string name) : name{name}, sid{SID(name.c_str())} {}
  virtual ~ICVar() {}
};

class CVarInt : public ICVar {
 public:
  int iVal;

  CVarInt(std::string name, int value);
  explicit CVarInt(std::string name);

  inline void SetVal(std::string strVal) override { iVal = stoi(strVal); }

  inline int GetVal() const { return iVal; }

  int GetType() const override { return CVAR_INT; }
};

class CVarFloat : public ICVar {
 public:
  float fVal;

  CVarFloat(std::string name, float value);
  explicit CVarFloat(std::string name);

  inline void SetVal(std::string strVal) override { fVal = stof(strVal); }

  inline float GetVal() const { return fVal; };

  inline int GetType() const override { return CVAR_FLOAT; }
};

class CVarString : public ICVar {
 public:
  std::string sVal;

  CVarString(std::string name, std::string value);
  explicit CVarString(std::string name);

  inline void SetVal(std::string strVal) override { sVal = strVal; }

  inline std::string GetVal() const { return sVal; }

  inline int GetType() const override { return CVAR_STRING; }
};

class CVarVector3 : public ICVar {
 public:
  Math::Vector3 v3Val;

  CVarVector3(std::string name, Math::Vector3 value);
  explicit CVarVector3(std::string name);

  inline void SetVal(std::string strVal) override {
    v3Val = Math::Vector3::FromString(strVal);
  }

  inline Math::Vector3 GetVal() const { return v3Val; }

  inline int GetType() const override { return CVAR_VECTOR3; }
};

}  // namespace Isetta

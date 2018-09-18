/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include <unordered_map>
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {
//#define REGISTER_DEC_TYPE(NAME) static CVarRegister<NAME> reg
//#define REGISTER_DEF_TYPE(NAME) static CVarRegister<NAME> NAME::reg(#NAME)

class ICVar {
 public:
  const std::string name;
  const StringId nameID;
  // const StringId typeID;

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

// struct ICVarFactory {
//  typedef std::unordered_map<StringId, ICVar* (*)()> map_type;
//
//  static ICVar* Instantiate(const StringId& s) {
//    map_type::iterator it = getMap()->find(s);
//    if (it == getMap()->end()) {
//      return 0;
//    }
//    return it->second();
//  }
//
// protected:
//  static map_type* getMap() {
//    if (!map) {
//      map = new map_type;
//    }
//    return map;
//  }
//
// private:
//  static map_type* map;
//};
//
// template <typename T>
// struct CVarRegister : ICVarFactory {
//  CVarRegister(const StringId& s) {
//    getMap()->insert(std::make_pair(s, &Instantiate<T>));
//  }
//};

}  // namespace Isetta

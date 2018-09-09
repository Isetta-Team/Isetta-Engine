/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <map>
#include "Core/Config/CVar.h"
#include "SID/sid.h"

namespace Isetta {
// #define REGISTER_INT(str)
class CVarRegistry {
 public:
  // TODO Singleton??

  static inline void RegisterVariable(ICVar* cvar) {
    registry.emplace(cvar->sid, cvar);
  }
  static inline bool UnregisterVariable(const StringId sid) {
    return registry.erase(sid);
  }
  static inline ICVar* Find(StringId sid) {
    auto it = registry.find(sid);
    if (it == registry.end()) {
      return nullptr;
    }
    return it->second;
  }

 private:
  static std::map<StringId, ICVar*> registry;
};
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <unordered_map>
#include "Core/Config/CVar.h"
#include "SID/sid.h"

namespace Isetta {
class CVarRegistry {
 public:
  static inline void RegisterVariable(ICVar* cvar) {
    registry.emplace(cvar->sid, cvar);
  }
  static inline bool UnregisterVariable(const StringId sid) {
    return registry.erase(sid);
  }
  inline ICVar* Find(const StringId sid) {
    auto it = registry.find(sid);
    if (it == registry.end()) {
      return nullptr;
    }
    return it->second;
  }

 private:
  static std::unordered_map<StringId, ICVar*> registry;
};
}  // namespace Isetta

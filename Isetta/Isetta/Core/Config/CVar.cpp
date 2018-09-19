/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Config/CVar.h"

#include <unordered_map>
#include "Core/Config/CVarRegistry.h"

namespace Isetta {
std::unordered_map<StringId, ICVar*> CVarRegistry::registry;

CVarString::CVarString(const std::string& name, const std::string& defaultValue)
    : ICVar(name), sVal{defaultValue} {
  CVarRegistry::RegisterVariable(this);
}

CVarString::CVarString(const std::string& name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(const std::string& name,
                         const Math::Vector3& defaultValue)
    : ICVar(name), v3Val{defaultValue} {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(const std::string& name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}
}  // namespace Isetta

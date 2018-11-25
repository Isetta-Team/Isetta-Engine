/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Config/CVar.h"

#include <unordered_map>
#include "Core/Config/CVarRegistry.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
// added explicit declarations so they can export correctly
template class ISETTA_API CVar<float>;

std::unordered_map<StringId, ICVar*> CVarRegistry::registry;
std::vector<std::string_view> CVarRegistry::keys;

CVarString::CVarString(const std::string& name,
                       const std::string_view defaultValue)
    : ICVar(name), sVal{defaultValue} {
  CVarRegistry::RegisterVariable(this);
}

CVarString::CVarString(const std::string_view name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(const std::string_view name,
                         const Math::Vector3& defaultValue)
    : ICVar(name), v3Val{defaultValue} {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(const std::string_view name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}
}  // namespace Isetta

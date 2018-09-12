/*
 * Copyright (c) 2018 Isetta
 */
#include "CVar.h"

#include "Core/Config/CVarRegistry.h"

namespace Isetta {
std::unordered_map<StringId, ICVar*> CVarRegistry::registry;

CVarInt::CVarInt(std::string name, int value) : ICVar(name), iVal{value} {
  CVarRegistry::RegisterVariable(this);
}

CVarInt::CVarInt(std::string name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}

CVarFloat::CVarFloat(std::string name, float value) : ICVar(name), fVal{value} {
  CVarRegistry::RegisterVariable(this);
}

CVarFloat::CVarFloat(std::string name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}

CVarString::CVarString(std::string name, std::string value)
    : ICVar(name), sVal{value} {
  CVarRegistry::RegisterVariable(this);
}

CVarString::CVarString(std::string name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(std::string name, Math::Vector3 value)
    : ICVar(name), v3Val{value} {
  CVarRegistry::RegisterVariable(this);
}

CVarVector3::CVarVector3(std::string name) : ICVar(name) {
  CVarRegistry::RegisterVariable(this);
}
}  // namespace Isetta

/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "CVar.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class Config {
 public:
  Config();

  CVarInt integerVar = CVarInt("integer", 0);
  CVarFloat floatVar = CVarFloat("float", 0);
  CVarString stringVar = CVarString("string", "test");
  CVarVector3 vector3Var = CVarVector3("vec3", Math::Vector3::one);
  CVarInt defaultValue = CVarInt("default", 10);

 private:
  void RemoveComments(std::string& line) const;
  bool OnlyWhitespace(const std::string& line) const;
  bool ValidLine(const std::string& line) const;
  void ExtractKey(std::string& key, const size_t& sepPos,
                  const std::string line);
  void ExtractValue(std::string& value, const size_t& sepPos,
                    const std::string line);
};
}  // namespace Isetta
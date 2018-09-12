/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/Config/CVar.h"
#include "Core/Config/CVarRegistry.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class Config {
 public:
  Config() = default;

  CVarInt integerVar{"integer", 0};
  CVarFloat floatVar{"float"};
  CVarString stringVar{"string", "test"};
  CVarVector3 vector3Var{"vec3", Math::Vector3::one};
  CVarInt defaultValue{"default", 10};

  void Read(std::string filepath);

 private:
  class CVarRegistry cvarsRegistry;

  void RemoveComments(std::string& line) const;
  bool OnlyWhitespace(const std::string& line) const;
  bool ValidLine(const std::string& line) const;
  void ExtractKey(std::string& key, const size_t& sepPos,
                  const std::string line);
  void ExtractValue(std::string& value, const size_t& sepPos,
                    const std::string line);
};
}  // namespace Isetta
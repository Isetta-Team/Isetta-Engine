/*
 * Copyright (c) 2018 Isetta
 */
#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Isetta {
void Config::Read(std::string filepath) {
  // TODO(jacob) config file location + filesystem?
  std::ifstream configFile(filepath);

  std::string line;
  while (std::getline(configFile, line)) {
    RemoveComments(line);
    if (OnlyWhitespace(line) || !ValidLine(line)) {
      continue;
    }
    size_t sepPos = line.find('=');
    std::string key, value;
    ExtractKey(key, sepPos, line);
    ExtractValue(value, sepPos, line);
    StringId keySid = SID(key.c_str());
    ICVar* cvar = cvarsRegistry.Find(keySid);
    if (cvar != nullptr) {
      switch (cvar->GetType()) {
        case CVAR_INT:
          static_cast<CVarInt*>(cvar)->SetVal(value);
          break;
        case CVAR_FLOAT:
          static_cast<CVarFloat*>(cvar)->SetVal(value);
          break;
        case CVAR_STRING:
          static_cast<CVarString*>(cvar)->SetVal(value);
          break;
        case CVAR_VECTOR3:
          static_cast<CVarVector3*>(cvar)->SetVal(value);
          break;
        default:
          throw std::exception("Config::Read Unexpected type");
      }
    }
  }
}

void Config::RemoveComments(std::string& line) const {
  auto it = line.find('#');
  if (it != std::string::npos) {
    line.erase(it);
  }
}

bool Config::OnlyWhitespace(const std::string& line) const {
  return (line.find_first_not_of(' ') == std::string::npos);
}

bool Config::ValidLine(const std::string& line) const {
  std::string tmp = line;
  tmp.erase(0, tmp.find_first_not_of("\t "));
  if (tmp[0] == '=') {
    return false;
  }

  int size = tmp.length();
  for (size_t i = tmp.find('=') + 1; i < size; i++) {
    if (tmp[i] != ' ') {
      return true;
    }
  }

  return false;
}
void Config::ExtractKey(std::string& key, const size_t& sepPos,
                        const std::string line) {
  key = line.substr(0, sepPos);
  size_t pos = key.find_first_of("\t ");
  if (pos != std::string::npos) {
    key.erase(pos);
  }
}

void Config::ExtractValue(std::string& value, const size_t& sepPos,
                          const std::string line) {
  value = line.substr(sepPos + 1);
  value.erase(0, value.find_first_not_of("\t "));
  value.erase(value.find_last_not_of("\t ") + 1);
}

}  // namespace Isetta
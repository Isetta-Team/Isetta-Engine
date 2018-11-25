/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Config/Config.h"

#include <string.h>
#include <functional>
#include <sstream>
#include <type_traits>
#include "Core/Config/ICVar.h"
#include "Core/DataStructures/Array.h"
#include "Core/Filesystem.h"

namespace Isetta {
void Config::Read(const std::string_view& filePath) {
  const char* contents = Filesystem::Instance().Read(filePath.data());
  ProcessFile(contents);
  delete contents;
}

void Config::ProcessFile(const char* contentBuffer) {
  char* nextToken;
  char* lines = strtok_s(const_cast<char*>(contentBuffer), "\n\r", &nextToken);
  while (lines != NULL) {
    std::string line(lines);
    RemoveComments(&line);
    if (OnlyWhitespace(line) || !ValidLine(line)) {
      lines = strtok_s(NULL, "\n", &nextToken);
      continue;
    }
    Size sepPos = line.find('=');
    std::string key, value;
    ExtractKey(&key, sepPos, line);
    ExtractValue(&value, sepPos, line);
    SetVal(key, value);
    lines = strtok_s(NULL, "\n", &nextToken);
  }
}

void Config::SetVal(const std::string& key, const std::string_view& value) {
  StringId keySid = SID(key.c_str());
  ICVar* cvar = cvarsRegistry.Find(keySid);
  if (cvar != nullptr) {
    cvar->SetVal(value.data());
  }
}

Array<std::string_view> Config::GetCommands() const {
  return Array<std::string_view>(cvarsRegistry.GetKeys());
}

void Config::RemoveComments(std::string* line) const {
  auto it = line->find('#');
  if (it != std::string::npos) {
    line->erase(it);
  }
  it = line->find('\r');
  if (it != std::string::npos) {
    line->erase(it);
  }
}

bool Config::OnlyWhitespace(const std::string_view& line) const {
  return (line.find_first_not_of(' ') == std::string::npos);
}

bool Config::ValidLine(const std::string_view& line) const {
  std::string tmp = line.data();
  tmp.erase(0, tmp.find_first_not_of("\t "));
  if (tmp[0] == '=') {
    return false;
  }

  int size = tmp.length();
  for (Size i = tmp.find('=') + 1; i < size; ++i) {
    if (tmp[i] != ' ') {
      return true;
    }
  }

  return false;
}
void Config::ExtractKey(std::string* key, const Size& sepPos,
                        const std::string_view line) {
  *key = line.substr(0, sepPos);
  Size pos = key->find_first_of("\t ");
  if (pos != std::string::npos) {
    key->erase(pos);
  }
}

void Config::ExtractValue(std::string* value, const Size& sepPos,
                          const std::string_view line) {
  *value = line.substr(sepPos + 1);
  value->erase(0, value->find_first_not_of("\t "));
  value->erase(value->find_last_not_of("\t ") + 1);
}

}  // namespace Isetta

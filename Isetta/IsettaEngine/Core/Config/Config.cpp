/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Config/Config.h"

#include <string.h>
#include <functional>
#include <sstream>
#include <type_traits>
#include "Core/Config/ICVar.h"
#include "Core/Filesystem.h"

namespace Isetta {
void Config::Read(const std::string& filePath) {
  const char* contents = Filesystem::Instance().Read(filePath);
  ProcessFile(contents);
  delete contents;
}

void Config::ProcessFile(const char* contentBuffer) {
  char* nextToken;
  char* lines = strtok_s(const_cast<char*>(contentBuffer), "\n", &nextToken);
  while (lines != NULL) {
    // printf("%s\n", lines);
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
    StringId keySid = SID(key.c_str());
    ICVar* cvar = cvarsRegistry.Find(keySid);
    if (cvar != nullptr) {
      cvar->SetVal(value);
    }
    lines = strtok_s(NULL, "\n", &nextToken);
  }
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
  for (Size i = tmp.find('=') + 1; i < size; i++) {
    if (tmp[i] != ' ') {
      return true;
    }
  }

  return false;
}
void Config::ExtractKey(std::string* key, const Size& sepPos,
                        const std::string line) {
  *key = line.substr(0, sepPos);
  Size pos = key->find_first_of("\t ");
  if (pos != std::string::npos) {
    key->erase(pos);
  }
}

void Config::ExtractValue(std::string* value, const Size& sepPos,
                          const std::string line) {
  *value = line.substr(sepPos + 1);
  value->erase(0, value->find_first_not_of("\t "));
  value->erase(value->find_last_not_of("\t ") + 1);
}

}  // namespace Isetta

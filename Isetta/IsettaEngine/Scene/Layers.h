/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <bitset>
#include <string>
#include <unordered_map>
#include "ISETTA_API.h"

class StringId;

namespace Isetta {
class ISETTA_API Layers {
 public:
  static const int LAYERS_CAPACITY = 32;
  static const int READONLY_LAYERS = 2;

 private:
  static std::string
      layers[LAYERS_CAPACITY]; /* = { "Default", "Ignore Raycast" };*/
  static std::unordered_map<StringId, int> layerIndex;
  static int size;

  Layers() = default;

  struct Constructor {
    Constructor();
  };
  static Constructor construct;

 public:
  static int NewLayer(const std::string_view layerName);

  static int NameToLayer(const std::string_view layerName);
  static std::string LayerToName(int layer);
  static int CheckLayer(int layer);

  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<int> mask);
  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<std::string> mask);
};
}  // namespace Isetta

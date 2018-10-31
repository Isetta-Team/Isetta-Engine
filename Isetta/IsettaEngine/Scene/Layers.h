/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <bitset>
#include <string>
#include <unordered_map>

class StringId;

namespace Isetta {
class Layers {
 public:
  static const int LAYERS_CAPACITY = 32;
  static const int READONLY_LAYERS = 2;

 private:
  static std::string
      layers[LAYERS_CAPACITY]; /* = { "Default", "Ignore Raycast" };*/
  static std::unordered_map<StringId, int> layerIndex;

  Layers() = default;

  struct Constructor {
    Constructor();
  };
  static Constructor construct;

 public:
  static void NameLayer(int layer, std::string layerName);
  static int NameToLayer(std::string layerName);
  static std::string LayerToName(int layer);
  static int CheckLayer(int layer);

  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<int> mask);
  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<std::string> mask);
};
}  // namespace Isetta

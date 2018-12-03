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
  /**
   * \brief Add a new layer
   * \return index of the new layer
   */
  static int NewLayer(const std::string_view layerName);

  /**
   * \brief Get the layer that has the input name
   * \return That layer's index
   */
  static int NameToLayer(const std::string_view layerName);
  /**
   * \brief Get the name of layer with input index
   */
  static std::string LayerToName(int layer);
  /**
   * \brief Check if the input layer is valid and within capacity
   */
  static int CheckLayer(int layer);

  /**
   * \brief Create a bit set according to the input int
   */
  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<int> mask);
  /**
   * \brief Create a bit set according to the input string
   */
  static std::bitset<LAYERS_CAPACITY> LayerMask(
      std::initializer_list<std::string> mask);
};
}  // namespace Isetta

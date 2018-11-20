/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Layers.h"
#include "SID/sid.h"

namespace Isetta {
std::string Layers::layers[LAYERS_CAPACITY] = {"Default", "Ignore Raycast"};
std::unordered_map<class StringId, int> Layers::layerIndex;

Layers::Constructor::Constructor() {
  layerIndex.reserve(LAYERS_CAPACITY);
  for (int i = 0; i < READONLY_LAYERS; ++i) {
    layerIndex.insert(std::make_pair(SID(layers[i].c_str()), i));
  }
}

void Layers::NameLayer(int layer, std::string layerName) {
  if (layer < READONLY_LAYERS)
    throw std::out_of_range(
        "Layer::NameLayer => Layer must not be READONLY Layer");
  if (layer >= LAYERS_CAPACITY)
    throw std::out_of_range(
        "Layer::NameLayer => Layer must not be larger than LAYERS_CAPACITY(" +
        std::to_string(LAYERS_CAPACITY) + ")");
  layers[layer] = layerName;
  layerIndex.insert(std::make_pair(SID(layers[layer].c_str()), layer));
}

int Layers::NameToLayer(std::string layerName) {
  auto it = layerIndex.find(SID(layerName.c_str()));
  if (it == layerIndex.end())
    throw std::out_of_range("Layer::StringToIndex => Layer name(" + layerName +
                            ") does not exist");
  return it->second;
}

std::string Layers::LayerToName(int layer) {
  if (layer >= LAYERS_CAPACITY)
    throw std::out_of_range(
        "Layer::IndexToString => Layer must not be larger than "
        "LAYERS_CAPACITY(" +
        std::to_string(LAYERS_CAPACITY) + ")");
  return layers[layer];
}

int Layers::CheckLayer(int layer) {
  if (layer >= LAYERS_CAPACITY)
    throw std::out_of_range(
        "Layer::CheckLayer => Layer must not be larger than "
        "LAYERS_CAPACITY(" +
        std::to_string(LAYERS_CAPACITY) + ")");
  return layer;
}

std::bitset<Layers::LAYERS_CAPACITY> Layers::LayerMask(
    std::initializer_list<int> mask) {
  std::bitset<LAYERS_CAPACITY> bits;
  for (const auto& i : mask) {
    bits.set(i, true);
  }
  return bits;
}
std::bitset<Layers::LAYERS_CAPACITY> Layers::LayerMask(
    std::initializer_list<std::string> mask) {
  std::bitset<LAYERS_CAPACITY> bits;
  for (const auto& i : mask) {
    bits.set(NameToLayer(i), true);
  }
  return bits;
}
}  // namespace Isetta

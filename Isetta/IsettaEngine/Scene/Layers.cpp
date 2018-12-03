/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Layers.h"

#include "SID/sid.h"
#include "Util.h"

namespace Isetta {
std::string Layers::layers[LAYERS_CAPACITY] = {"Default", "Ignore Raycast"};
std::unordered_map<class StringId, int> Layers::layerIndex;
int Layers::size = READONLY_LAYERS;

Layers::Constructor::Constructor() {
  layerIndex.reserve(LAYERS_CAPACITY);
  for (int i = 0; i < READONLY_LAYERS; ++i) {
    layerIndex.insert({SID(layers[i].c_str()), i});
  }
}

int Layers::NewLayer(const std::string_view layerName) {
  if (size == LAYERS_CAPACITY)
    throw std::out_of_range(
        "Layer::NameLayer => Layer must not be larger than LAYERS_CAPACITY(" +
        std::to_string(LAYERS_CAPACITY) + ")");
  layers[size] = layerName;
  layerIndex.insert({SID(layerName.data()), size});
  return size++;
}

int Layers::NameToLayer(const std::string_view layerName) {
  auto it = layerIndex.find(SID(layerName.data()));
  if (it == layerIndex.end())
    throw std::out_of_range(
        Util::StrFormat("Layer::StringToIndex => Layer name(%s) does not exist",
                        layerName.data()));
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

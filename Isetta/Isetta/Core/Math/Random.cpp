/*
 * Copyright (c) 2018 Isetta
 */
#include "Random.h"

namespace Isetta::Math {
RandomGenerator<int> Random::GetRandomGenerator(int start, int end) {
  return RandomGenerator<int>(start, end);
}

RandomGenerator<int> Random::GetRandomGenerator(int start, int end, int seed) {
  return RandomGenerator<int>(start, end, seed);
}

RandomGenerator<float> Random::GetRandomGenerator(float start, float end) {
  return RandomGenerator<float>(start, end);
}

RandomGenerator<float> Random::GetRandomGenerator(float start, float end,
                                                  int seed) {
  return RandomGenerator<float>(start, end, seed);
}

float Random::GetRandom01() { return randomFloatGen.GetValue(); }

RandomGenerator<float> Random::randomFloatGen =
    RandomGenerator<float>(0.f, 1.f);

}  // namespace Isetta::Math

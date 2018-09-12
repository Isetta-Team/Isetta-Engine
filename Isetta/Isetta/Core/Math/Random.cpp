/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Random.h"

namespace Isetta::Math {
RandomGeneratorInt Random::GetRandomGenerator(int start, int end) {
  return RandomGeneratorInt(start, end);
}

RandomGeneratorInt Random::GetRandomGenerator(int start, int end, int seed) {
  return RandomGeneratorInt(start, end, seed);
}

RandomGenerator Random::GetRandomGenerator(float start, float end) {
  return RandomGenerator(start, end);
}

RandomGenerator Random::GetRandomGenerator(float start, float end, int seed) {
  return RandomGenerator(start, end, seed);
}

float Random::GetRandom01() { return randomFloatGen.GetValue(); }

RandomGenerator Random::randomFloatGen = RandomGenerator(0.f, 1.f);

}  // namespace Isetta::Math

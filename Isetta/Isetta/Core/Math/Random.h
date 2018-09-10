// "Copyright [2018] Isetta"

#pragma once

#include <random>
#include <type_traits>
#include <variant>

namespace Isetta::Math {

class RandomGeneratorInt {
  friend class Random;
  std::mt19937_64 generator;
  std::uniform_int_distribution<int> distribution;
  RandomGeneratorInt(int start, int end)
      : generator{std::random_device{}()},
        distribution{std::uniform_int_distribution<int>(start, end)} {}
  RandomGeneratorInt(int start, int end, int seed)
      : generator{seed},
        distribution{std::uniform_int_distribution<int>(start, end)} {}

 public:
  int GetValue() { return distribution(generator); }
};

class RandomGenerator {
  friend class Random;
  std::mt19937_64 generator;
  std::uniform_real_distribution<float> distribution;
  RandomGenerator(float start, float end)
      : generator{std::random_device{}()},
        distribution{std::uniform_real_distribution<float>(start, end)} {}
  RandomGenerator(float start, float end, int seed)
      : generator{seed},
        distribution{std::uniform_real_distribution<float>(start, end)} {}

 public:
  float GetValue() { return distribution(generator); }
};

class Random {
 public:
  static RandomGeneratorInt GetRandomGenerator(int start, int end);
  static RandomGeneratorInt GetRandomGenerator(int start, int end, int seed);
  static RandomGenerator GetRandomGenerator(float start, float end);
  static RandomGenerator GetRandomGenerator(float start, float end,
                                                   int seed);
  static float GetRandom01();

 private:
  static RandomGenerator randomFloatGen;
};
}  // namespace Isetta::Math

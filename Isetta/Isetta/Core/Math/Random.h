// "Copyright [2018] Isetta"

#pragma once

#include <random>
#include <type_traits>
#include <variant>

namespace Isetta::Math {
template <class T>
class RandomGenerator {
 public:
  RandomGenerator() = delete;
  RandomGenerator(const RandomGenerator&) = delete;
  RandomGenerator(RandomGenerator&&) = delete;
  RandomGenerator& operator=(const RandomGenerator&) = delete;
  RandomGenerator& operator=(RandomGenerator&&) = delete;
};

template <>
class RandomGenerator<int> {
  friend class Random;
  std::mt19937_64 generator;
  std::uniform_int_distribution<int> distribution;
  RandomGenerator(int start, int end)
      : generator{std::random_device{}()},
        distribution{std::uniform_int_distribution<int>(start, end)} {}
  RandomGenerator(int start, int end, int seed)
      : generator{seed},
        distribution{std::uniform_int_distribution<int>(start, end)} {}

 public:
  int GetValue() { return distribution(generator); }
};

template <>
class RandomGenerator<float> {
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
  static RandomGenerator<int> GetRandomGenerator(int start, int end);
  static RandomGenerator<int> GetRandomGenerator(int start, int end, int seed);
  static RandomGenerator<float> GetRandomGenerator(float start, float end);
  static RandomGenerator<float> GetRandomGenerator(float start, float end,
                                                   int seed);
  static float GetRandom01();

 private:
  static RandomGenerator<float> randomFloatGen;
};
}  // namespace Isetta::Math

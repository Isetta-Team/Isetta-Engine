// "Copyright [2018] Isetta"

#pragma once

#include <random>
#include "ISETTA_API.h"

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
  /**
   * \brief Get next random value
   */
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
  /**
   * \brief Get next random value
   */
  float GetValue() { return distribution(generator); }
};

class ISETTA_API Random {
 public:
  /**
   * \brief Get a random generator that generates number in range [start, end)
   * \param start Start number
   * \param end End number
   */
  static RandomGeneratorInt GetRandomGenerator(int start, int end);
  /**
   * \brief Get a random generator with a specific seed that generates number in
   * range [start, end)
   * \param start Start number
   * \param end End number
   * \param seed Specific seed
   */
  static RandomGeneratorInt GetRandomGenerator(int start, int end, int seed);
  /**
   * \brief Get a random generator that generates number in range [start, end)
   * \param start Start number
   * \param end End number
   */
  static RandomGenerator GetRandomGenerator(float start, float end);
  /**
   * \brief Get a random generator with a specific seed that generates number in
   * range [start, end)
   * \param start Start number
   * \param end End number
   * \param seed Specific seed
   */
  static RandomGenerator GetRandomGenerator(float start, float end, int seed);
  /**
   * \brief Use a default random generator to generate a random number in [0, 1)
   */
  static float GetRandom01();

 private:
  static RandomGenerator randomFloatGen;
};
}  // namespace Isetta::Math

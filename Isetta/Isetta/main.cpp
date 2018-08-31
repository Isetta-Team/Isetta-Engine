// "Copyright [2018] Isetta"
#include <iostream>
#include "Math/Random.h"

int main() {
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;
  std::cin.get();
}
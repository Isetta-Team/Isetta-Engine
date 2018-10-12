#include <iostream>
#include "Core/DataStructures/RingBuffer.h"
#include "EngineLoop.h"

int main() {
  Isetta::RingBuffer<int> ring;
  Isetta::EngineLoop loop;
  loop.Run();
  return 0;
}
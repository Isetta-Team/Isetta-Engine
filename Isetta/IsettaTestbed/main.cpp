#include <iostream>
#include "Core/DataStructures/RingBuffer.h"
#include "EngineLoop.h"

int main() {
  Isetta::EngineLoop::Instance().Run();
  return 0;
}
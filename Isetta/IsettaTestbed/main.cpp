#include <iostream>
#include "EngineLoop.h"
#include "Util.h"

int main() {
  Isetta::EngineLoop loop;
  std::cout << Isetta::Util::MegaBytesFromBytes(1024 * 1024) << std::endl;
  loop.Run();
  return 0;
}
#include "Core/Memory/Memory.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MemoryTest {

TEST_CLASS(MemoryTest) {
  public:
  TEST_METHOD(AllocatedAligned) {
    U8 alignments[] =  {2, 4, 8, 16, 32, 64, 128};

    for (U8& alignment : alignments) {
      for (SizeInt size = 1; size < 1000; size++) {
        void* mem = MemoryManager::AllocateAligned(size, alignment);
        U8 address = reinterpret_cast<U8>(mem);
        Assert::IsTrue(address % alignment == 0);
        MemoryManager::FreeAligned(mem);
      }
    }
  }

};

}

/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/DataStructures/RingBuffer.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace DataStructuresTest {

TEST_CLASS(RingBufferTest){public :
                               TEST_METHOD(Get){RingBuffer<int> rb{1, 2, 3};
Assert::AreEqual(1, rb.Get());
Assert::AreEqual(3, rb.GetCapacity());
}  // namespace DataStructuresTest

TEST_METHOD(Put) {
  RingBuffer<int> rb{{1, 2, 3}, 4};
  rb.Put(4);
  Assert::IsTrue(rb.IsFull());
  Assert::AreEqual(1, rb.Get());
}

TEST_METHOD(GetBack) {
  RingBuffer<int> rb{1, 2, 3};
  Assert::AreEqual(3, rb.GetBack());
  Assert::AreEqual(3, rb.GetCapacity());
}

TEST_METHOD(PutFront) {
  RingBuffer<int> rb{{1, 2, 3}, 4};
  rb.PutFront(4);
  Assert::IsTrue(rb.IsFull());
  Assert::AreEqual(4, rb.Get());
}

TEST_METHOD(Clear) {
  RingBuffer<int> rb{1, 2, 3};
  rb.Clear();
  Assert::IsTrue(rb.IsEmpty());
}

TEST_METHOD(IsEmpty) {
  RingBuffer<int> rb{1, 2, 3};
  Assert::IsFalse(rb.IsEmpty());
  rb.Get();
  rb.Get();
  rb.Get();
  Assert::IsTrue(rb.IsEmpty());
}

TEST_METHOD(IsFull) {
  RingBuffer<int> rb{1, 2, 3};
  Assert::IsTrue(rb.IsFull());
  rb.Get();
  Assert::IsFalse(rb.IsFull());
}

TEST_METHOD(GetCapacity) {
  RingBuffer<int> rb(5);
  Assert::AreEqual(5, rb.GetCapacity());
}

TEST_METHOD(GetLength) {
  RingBuffer<int> rb{{1, 2, 3}, 4};
  Assert::AreEqual(3, rb.GetLength());
}
}
;
}  // namespace DataStructuresTest

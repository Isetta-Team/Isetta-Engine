/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/DataStructures/Array.h"
#include "Core/DataStructures/PriorityQueue.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace DataStructuresTest {
TEST_CLASS(PriorityQueueTest){
  private :     //
      public :  //
          TEST_METHOD(Constructors){{PriorityQueue<int, Less> queue;
Assert::AreEqual(0, queue.Size());
}  // namespace DataStructuresTest
{
  PriorityQueue<int, Less> queue{1, 2, 3};
  Assert::AreEqual(1, queue.Top());
  Assert::AreEqual(3, queue.Size());
}
{
  Array<int> arr{1, 2, 3};
  PriorityQueue<int, Less> queue{arr};
  Assert::AreEqual(1, queue.Top());
  Assert::AreEqual(3, queue.Size());
}
//{
//  Array<int> arr{1, 2, 3, 4};
//  PriorityQueue<int, Less> queue{arr.begin() + 1, arr.end() - 1};
//  Assert::AreEqual(2, queue.Top());
//  Assert::AreEqual(2, queue.Size());
//}
{
  int arr[] = {1, 2, 3};
  PriorityQueue<int, Less> queue{arr, arr + 3};
  Assert::AreEqual(1, queue.Top());
  Assert::AreEqual(3, queue.Size());
}
}

TEST_METHOD(Emplace) {
  PriorityQueue<int, Less> queue{1, 2, 3};
  queue.Emplace(0);
  Assert::AreEqual(0, queue.Top());
  Assert::AreEqual(4, queue.Size());
}

TEST_METHOD(IsEmpty) {
  PriorityQueue<int, Less> empty;
  Assert::AreEqual(true, empty.IsEmpty());
  PriorityQueue<int, Less> filled{1};
  Assert::AreEqual(false, filled.IsEmpty());
}

TEST_METHOD(Pop) {
  PriorityQueue<int, Less> queue{1, 2, 3};
  queue.Pop();
  Assert::AreEqual(2, queue.Size());
  Assert::AreEqual(2, queue.Top());
}

TEST_METHOD(Push) {
  PriorityQueue<int, Less> queue{1, 2, 3};
  queue.Push(0);
  Assert::AreEqual(4, queue.Size());
  Assert::AreEqual(0, queue.Top());
  queue.Push(9);
  Assert::AreEqual(5, queue.Size());
  Assert::AreEqual(0, queue.Top());
}

TEST_METHOD(Swap) {
  PriorityQueue<int, Less> queueA{1, 2, 3};
  PriorityQueue<int, Less> queueB{4, 3};
  queueA.Swap(queueB);
  Assert::AreEqual(2, queueA.Size());
  Assert::AreEqual(3, queueB.Size());
}

TEST_METHOD(TestMe) {
  PriorityQueue<int, Less> queue{1, 2, 3};
  // queue.Emplace(5);
  queue.Push(3);
  // Array<int> arr{1, 2, 3};
  // arr.EmplaceBack(1);
}
}
;  // namespace DataStructuresTest
}
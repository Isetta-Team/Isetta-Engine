/*
 * Copyright (c) 2018 Isetta
 */
#include <string>
#include "Core/DataStructures/Array.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace DataStructuresTest {
TEST_CLASS(ArrayTest){private :     //
                          public :  //
                              TEST_METHOD(Constructors){{Array<int> vec;
Assert::AreEqual(0, (int)vec.Size());
Assert::AreEqual(0, (int)vec.Capacity());
}  // namespace DataStructuresTest
{
  Array<int> vec(5);
  Assert::AreEqual(0, (int)vec.Size());
  Assert::AreEqual(5, (int)vec.Capacity());
}
{
  Array<int> vec(5, 2);
  Assert::AreEqual(5, (int)vec.Size());
  Assert::AreEqual(5, (int)vec.Capacity());
  Assert::AreEqual(2, vec[0]);
}
{
  Array<int> vec{1, 2, 3};
  Assert::AreEqual(3, (int)vec.Size());
  Assert::AreEqual(3, (int)vec.Capacity());
  Assert::AreEqual(2, vec[1]);
}
{
  Array<int> dummy{1, 2, 3, 4};
  Array<int> vec{dummy.begin() + 1, dummy.end() - 1};
  Assert::AreEqual(2, (int)vec.Size());
  Assert::AreEqual(2, (int)vec.Capacity());
  Assert::AreEqual(3, vec[1]);
}
}

TEST_METHOD(Assignment) {
  Array<std::string_view> vecA{"1", "2", "300"};
  vecA.PushBack("test");
  Array<std::string_view> vecB = vecA;
  Array<std::string_view>& vecC = vecB;
  Array<std::string_view> vec;
  vec.PushBack("test_1");
  vec.PushBack("test_2");
}

TEST_METHOD(PtrType) {
  Array<int> vecInt;
  vecInt.PushBack(1);
  vecInt.PushBack(2);

  Array<std::string> vecStr;
  vecStr.PushBack("test");
  vecStr.PushBack("test");
}

TEST_METHOD(SizeCapacity) {
  Array<int> vec{1, 2, 3};
  Assert::AreEqual(3, (int)vec.Size());
  Assert::AreEqual(3, (int)vec.Capacity());
  Assert::AreEqual(false, vec.IsEmpty());
}

TEST_METHOD(Empty) {
  Array<int> vec;
  Assert::AreEqual(true, vec.IsEmpty());
  Assert::AreEqual(0, (int)vec.Size());
  Assert::AreEqual(0, (int)vec.Capacity());
}

TEST_METHOD(Resize) {
  Array<int> vec{1, 2, 3};
  vec.Resize(1);
  Assert::AreEqual(1, (int)vec.Size());
  Assert::AreEqual(1, vec[0]);

  vec.Resize(4, 5);
  Assert::AreEqual(4, (int)vec.Size());
  Assert::AreEqual(1, vec[0]);
  Assert::AreEqual(5, vec[2]);
  Assert::AreEqual(5, vec[3]);
}

TEST_METHOD(Reserve) {
  Array<int> vec;
  vec.Reserve(5);
  Assert::AreEqual(5, (int)vec.Capacity());
}
TEST_METHOD(Shrink) {
  Array<int> vec(10);
  vec.Resize(5);
  Assert::AreEqual(5, (int)vec.Size());
  Assert::AreEqual(10, (int)vec.Capacity());
  vec.Shrink();
  Assert::AreEqual(5, (int)vec.Capacity());
  Assert::AreEqual(5, (int)vec.Size());
}

TEST_METHOD(Access) {
  Array<int> vec{1, 2, 3};
  Assert::AreEqual(1, vec[0]);
  Assert::AreEqual(2, vec.At(1));
  Assert::AreEqual(1, vec.Front());
  Assert::AreEqual(3, vec.Back());
}

TEST_METHOD(AccessAssign) {
  Array<int> vec{1, 2, 3, 4};
  vec.Front() = 4;
  Assert::AreEqual(4, vec.Front());
  vec[1] = 3;
  Assert::AreEqual(3, vec[1]);
  vec.At(2) = 2;
  Assert::AreEqual(2, vec.At(2));
  vec.Back() = 1;
  Assert::AreEqual(1, vec.Back());
}

TEST_METHOD(Assign) {
  Array<int> vec{1, 2, 3};
  vec.Assign(2, 0);
  Assert::AreEqual(0, vec[0]);
  Assert::AreEqual(0, vec[1]);
  Assert::AreEqual(2, (int)vec.Size());
  vec.Assign({100, 200, 300, 400});
  Assert::AreEqual(4, (int)vec.Size());
  for (int i = 0; i < 4; i++) Assert::AreEqual(100 * (i + 1), vec[i]);
  int arr[] = {12, 13, 14};
  vec.Assign(arr, arr + 3);
}

TEST_METHOD(PushPop) {
  Array<int> vec;
  vec.PushBack(1);
  vec.PushBack(2);
  vec.PushBack(3);
  Assert::AreEqual(1, vec[0]);
  Assert::AreEqual(2, vec[1]);
  Assert::AreEqual(3, vec[2]);
  Assert::AreEqual(3, (int)vec.Size());
  Assert::AreEqual(4, (int)vec.Capacity());
  vec.PopBack();
  Assert::AreEqual(1, vec[0]);
  Assert::AreEqual(2, vec[1]);
  Assert::AreEqual(2, (int)vec.Size());
  Assert::AreEqual(4, (int)vec.Capacity());
}

TEST_METHOD(InsertSingle) {
  Array<int> vec(3, 100);
  Array<int>::iterator it = vec.begin();
  it = vec.Insert(it, 200);
  Assert::AreEqual(200, vec[0]);
  Assert::AreEqual(4, (int)vec.Size());
}
TEST_METHOD(InsertMultiple) {
  Array<int> vec(3, 100);
  Array<int>::iterator it = vec.begin();
  vec.Insert(it, 2, 300);
  Assert::AreEqual(300, vec[0]);
  Assert::AreEqual(300, vec[1]);
  Assert::AreEqual(100, vec[3]);
  Assert::AreEqual(100, vec[4]);
  Assert::AreEqual(5, (int)vec.Size());
}
TEST_METHOD(InsertVector) {
  Array<int> vec(3, 100);
  Array<int>::iterator it = vec.begin();
  Array<int> vecB(2, 400);
  vec.Insert(it + 2, vecB.begin(), vecB.end());
  Assert::AreEqual(100, vec[0]);
  Assert::AreEqual(100, vec[1]);
  Assert::AreEqual(400, vec[2]);
  Assert::AreEqual(400, vec[3]);
  Assert::AreEqual(100, vec[4]);
  Assert::AreEqual(5, (int)vec.Size());
}
TEST_METHOD(InsertArray) {
  Array<int> vec(3, 100);
  Array<int>::iterator it = vec.begin();
  int arr[] = {501, 502, 503};
  vec.Insert(vec.begin(), arr, arr + 3);
  Assert::AreEqual(501, vec[0]);
  Assert::AreEqual(502, vec[1]);
  Assert::AreEqual(503, vec[2]);
  Assert::AreEqual(100, vec[3]);
  Assert::AreEqual(100, vec[4]);
  Assert::AreEqual(100, vec[5]);
  Assert::AreEqual(6, (int)vec.Size());
}

TEST_METHOD(EraseSingle) {
  Array<int> vec{1, 2, 3, 4};
  vec.Erase(vec.begin() + 1);
  Assert::AreEqual(3, (int)vec.Size());
  Assert::AreEqual(3, vec[1]);
  Assert::AreEqual(4, vec[2]);
}

TEST_METHOD(EraseRange) {
  {
    Array<int> vec{1, 2, 3, 4};
    vec.Erase(vec.begin() + 1, vec.end());
    Assert::AreEqual(1, (int)vec.Size());
    Assert::AreEqual(1, vec[0]);
  }
  {
    Array<int> vec{1, 2, 3, 4};
    vec.Erase(vec.begin(), vec.end() - 1);
    Assert::AreEqual(1, (int)vec.Size());
    Assert::AreEqual(4, vec[0]);
  }
}

TEST_METHOD(Emplace) {
  Array<int> vec = {10, 20, 30};

  auto it = vec.Emplace(vec.begin() + 1, 100);
  vec.Emplace(it, 200);
  vec.Emplace(vec.end(), 300);

  Assert::AreEqual(10, vec[0]);
  Assert::AreEqual(200, vec[1]);
  Assert::AreEqual(100, vec[2]);
  Assert::AreEqual(20, vec[3]);
  Assert::AreEqual(30, vec[4]);
  Assert::AreEqual(300, vec[5]);
  Assert::AreEqual(6, (int)vec.Size());

  Array<int> vecBack = {10, 20, 30};
  vecBack.EmplaceBack(100);
  vecBack.EmplaceBack(200);

  Assert::AreEqual(10, vecBack[0]);
  Assert::AreEqual(20, vecBack[1]);
  Assert::AreEqual(30, vecBack[2]);
  Assert::AreEqual(100, vecBack[3]);
  Assert::AreEqual(200, vecBack[4]);
  Assert::AreEqual(5, (int)vecBack.Size());
}

TEST_METHOD(Swap) {
  Array<int> vecA{1, 2, 3};
  Array<int> vecB{10, 20};
  vecA.Swap(vecB);
  Assert::AreEqual(10, vecA[0]);
  Assert::AreEqual(20, vecA[1]);
  Assert::AreEqual(1, vecB[0]);
  Assert::AreEqual(2, vecB[1]);
  Assert::AreEqual(3, vecB[2]);
  Assert::AreEqual(2, (int)vecA.Size());
  Assert::AreEqual(2, (int)vecA.Capacity());
  Assert::AreEqual(3, (int)vecB.Size());
  Assert::AreEqual(3, (int)vecB.Capacity());
  Array<int>().Swap(vecA);
  Assert::AreEqual(0, (int)vecA.Size());
  Assert::AreEqual(0, (int)vecA.Capacity());
}

TEST_METHOD(Clear) {
  Array<int> vec{1, 2, 3};
  vec.Clear();
  Assert::AreEqual(0, (int)vec.Size());
  Assert::AreNotEqual(0, (int)vec.Capacity());
}
}
;  // namespace DataStructuresTest
}  // namespace DataStructuresTest
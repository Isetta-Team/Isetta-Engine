#include <cmath>
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector3Int.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Vector3IntTest){
  public :

      TEST_METHOD(Magnitude){Math::Vector3Int vec{1, 1, 1};
Assert::AreEqual(sqrt(3.f), vec.Magnitude());
}  // namespace MathTest

TEST_METHOD(SqrMagnitude) {
  Math::Vector3Int vec{1, 1, 1};
  Assert::AreEqual(3, vec.SqrMagnitude());
}

TEST_METHOD(Normalized) {
  Math::Vector3Int vec{5, 0, 0};
  Assert::IsTrue(Math::Vector3::left == vec.Normalized());
}

TEST_METHOD(Dot) {
  Math::Vector3Int vec1{5, 1, 3};
  Math::Vector3Int vec2(2, 5, 1);
  Assert::AreEqual(18, Math::Vector3Int::Dot(vec1, vec2));
}

TEST_METHOD(Cross) {
  Math::Vector3Int vec1{1, 3, -4};
  Math::Vector3Int vec2(2, -5, 8);
  Assert::IsTrue(Math::Vector3Int::Cross(vec1, vec2) ==
                 Math::Vector3Int(4, -16, -11));
}

TEST_METHOD(Distance) {
  Math::Vector3Int vec1{1, 0, 0};
  Math::Vector3Int vec2(0, 1, 0);
  Assert::AreEqual(Math::Vector3Int::Distance(vec1, vec2), sqrt(2.f));
}

TEST_METHOD(Scale) {
  Math::Vector3Int vec1{1, -2, 4};
  Math::Vector3Int vec2(5, 3, 6);
  Assert::IsTrue(Math::Vector3Int::Scale(vec1, vec2) ==
                 Math::Vector3Int(5, -6, 24));
}
}
;
}
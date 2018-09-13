#include <cmath>
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector2Int.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Vector2IntTest){
  public :

      TEST_METHOD(Magnitude){Math::Vector2Int vec{1, 1};
Assert::AreEqual(sqrt(2.f), vec.Magnitude());
}  // namespace MathTest

TEST_METHOD(SqrMagnitude) {
  Math::Vector2Int vec{1, 1};
  Assert::AreEqual(2, vec.SqrMagnitude());
}

TEST_METHOD(Normalized) {
  Math::Vector2Int vec{5, 0};
  Assert::IsTrue(Math::Vector2::right == vec.Normalized());
}

TEST_METHOD(Dot) {
  Math::Vector2Int vec1{5, 1};
  Math::Vector2Int vec2(2, 5);
  Assert::AreEqual(15, Math::Vector2Int::Dot(vec1, vec2));
}

TEST_METHOD(Cross) {
  Math::Vector2Int vec1{1, 3};
  Math::Vector2Int vec2(2, -5);
  Assert::AreEqual(-11, Math::Vector2Int::Cross(vec1, vec2));
}

TEST_METHOD(Distance) {
  Math::Vector2Int vec1{1, 0};
  Math::Vector2Int vec2(0, 1);
  Assert::AreEqual(Math::Vector2Int::Distance(vec1, vec2), sqrt(2.f));
}

TEST_METHOD(Scale) {
  Math::Vector2Int vec1{1, -2};
  Math::Vector2Int vec2(5, 3);
  Assert::IsTrue(Math::Vector2Int::Scale(vec1, vec2) ==
                 Math::Vector2Int(5, -6));
}
}
;
}
#include <cmath>
#include <limits>
#include "Core/Math/Vector2.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Vector2Test){public :

                            TEST_METHOD(Magnitude){Math::Vector2 vec{1.f, 1.f};
Assert::AreEqual(sqrt(2.f), vec.Magnitude());
}  // namespace MathTest

TEST_METHOD(SqrMagnitude) {
  Math::Vector2 vec{1.f, 1.f};
  Assert::AreEqual(2.f, vec.SqrMagnitude());
}

TEST_METHOD(Normalized) {
  Math::Vector2 vec{5.f, 0.f};
  Assert::IsTrue(Math::Vector2::right == vec.Normalized());
}

TEST_METHOD(Normalize) {
  Math::Vector2 vec{5.f, 0.f};
  vec.Normalize();
  Assert::IsTrue(Math::Vector2::right == vec);
}

TEST_METHOD(Equals) {
  Math::Vector2 vec1{5.f, 0.f};
  Math::Vector2 vec2(5.f + FLT_EPSILON, 0.f);
  Assert::IsTrue(vec1 == vec2);
}

TEST_METHOD(Dot) {
  Math::Vector2 vec1{5.f, 1.f};
  Math::Vector2 vec2(2.f, 5.f);
  Assert::AreEqual(15.f, Math::Vector2::Dot(vec1, vec2));
}

TEST_METHOD(Cross) {
  Math::Vector2 vec1{1.f, 3.f};
  Math::Vector2 vec2(2.f, -5.f);
  Assert::AreEqual(-11.f, Math::Vector2::Cross(vec1, vec2));
}

TEST_METHOD(Lerp) {
  Math::Vector2 vec1{1.f, 0.f};
  Math::Vector2 vec2(0.f, 1.f);
  Assert::IsTrue(Math::Vector2::Lerp(vec1, vec2, 0.5f) ==
                 Math::Vector2(0.5f, 0.5f));
}

TEST_METHOD(Distance) {
  Math::Vector2 vec1{1.f, 0.f};
  Math::Vector2 vec2(0.f, 1.f);
  Assert::AreEqual(Math::Vector2::Distance(vec1, vec2), sqrt(2.f));
}

TEST_METHOD(Project) {
  Math::Vector2 vec1{1.f, 0.f};
  Math::Vector2 vec2(0.f, 1.f);
  Assert::IsTrue(Math::Vector2::Project(vec1, vec2) == Math::Vector2::zero);
}

TEST_METHOD(Reflect) {
  Math::Vector2 vec1{1.f, -1.f};
  Math::Vector2 vec2(0.f, 1.f);
  Assert::IsTrue(Math::Vector2::Reflect(vec1, vec2) == Math::Vector2(1.f, 1.f));
}

TEST_METHOD(Scale) {
  Math::Vector2 vec1{1.f, -2.f};
  Math::Vector2 vec2(5.f, 3.f);
  Assert::IsTrue(Math::Vector2::Scale(vec1, vec2) == Math::Vector2(5.f, -6.f));
}

TEST_METHOD(Slerp) {
  Math::Vector2 vec1{1.f, 0.f};
  Math::Vector2 vec2(0.f, 1.f);
  Assert::IsTrue(Math::Vector2::Slerp(vec1, vec2, 0.f) ==
                 Math::Vector2(1.f, 0.f));
}
}
;
}
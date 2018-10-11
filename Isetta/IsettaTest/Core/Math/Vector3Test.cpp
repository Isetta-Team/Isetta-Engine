#include <cmath>
#include <limits>
#include "Core/Math/Vector3.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Vector3Test){
  public :

      TEST_METHOD(Magnitude){Math::Vector3 vec{1.f, 1.f, 1.f};
Assert::AreEqual(sqrt(3.f), vec.Magnitude());
}  // namespace MathTest

TEST_METHOD(SqrMagnitude) {
  Math::Vector3 vec{1.f, 1.f, 1.f};
  Assert::AreEqual(3.f, vec.SqrMagnitude());
}

TEST_METHOD(Normalized) {
  Math::Vector3 vec{5.f, 0.f, 0.f};
  Assert::IsTrue(Math::Vector3::left == vec.Normalized());
}

TEST_METHOD(Normalize) {
  Math::Vector3 vec{5.f, 0.f, 0.f};
  vec.Normalize();
  Assert::IsTrue(Math::Vector3::left == vec);
}

TEST_METHOD(Equals) {
  Math::Vector3 vec1{5.f, 0.f, 0.f};
  Math::Vector3 vec2(5.f + FLT_EPSILON, 0.f, 0.f);
  Assert::IsTrue(vec1 == vec2);
}

TEST_METHOD(Dot) {
  Math::Vector3 vec1{5.f, 1.f, 3.f};
  Math::Vector3 vec2(2.f, 5.f, 1.f);
  Assert::AreEqual(18.f, Math::Vector3::Dot(vec1, vec2));
}

TEST_METHOD(Cross) {
  Math::Vector3 vec1{1.f, 3.f, -4.f};
  Math::Vector3 vec2(2.f, -5.f, 8.f);
  Assert::IsTrue(Math::Vector3::Cross(vec1, vec2) ==
                 Math::Vector3(4.f, -16.f, -11.f));
}

TEST_METHOD(Lerp) {
  Math::Vector3 vec1{1.f, 0.f, 0.f};
  Math::Vector3 vec2(0.f, 1.f, 0.f);
  Assert::IsTrue(Math::Vector3::Lerp(vec1, vec2, 0.5f) ==
                 Math::Vector3(0.5f, 0.5f, 0.f));
}

TEST_METHOD(Distance) {
  Math::Vector3 vec1{1.f, 0.f, 0.f};
  Math::Vector3 vec2(0.f, 1.f, 0.f);
  Assert::AreEqual(Math::Vector3::Distance(vec1, vec2), sqrt(2.f));
}

TEST_METHOD(Project) {
  Math::Vector3 vec1{1.f, 0.f, 0.f};
  Math::Vector3 vec2(0.f, 1.f, 0.f);
  Assert::IsTrue(Math::Vector3::Project(vec1, vec2) == Math::Vector3::zero);
}

TEST_METHOD(Reflect) {
  Math::Vector3 vec1{1.f, -1.f, 0.f};
  Math::Vector3 vec2(0.f, 1.f, 0.f);
  Assert::IsTrue(Math::Vector3::Reflect(vec1, vec2) ==
                 Math::Vector3(1.f, 1.f, 0.f));
}

TEST_METHOD(Scale) {
  Math::Vector3 vec1{1.f, -2.f, 4.f};
  Math::Vector3 vec2(5.f, 3.f, 6.f);
  Assert::IsTrue(Math::Vector3::Scale(vec1, vec2) ==
                 Math::Vector3(5.f, -6.f, 24.f));
}

TEST_METHOD(Slerp) {
  Math::Vector3 vec1{1.f, 0.f, 0.f};
  Math::Vector3 vec2(0.f, 1.f, 0.f);
  Assert::IsTrue(Math::Vector3::Slerp(vec1, vec2, 0.f) ==
                 Math::Vector3(1.f, 0.f, 0.f));
}

TEST_METHOD(AngleRad) {
  Math::Vector3 a{1, 0, 0};
  Math::Vector3 b{1, 1, 0};
  Assert::IsTrue(Math::Vector3::AngleDeg(a, b) == 45);
}
}
;
}
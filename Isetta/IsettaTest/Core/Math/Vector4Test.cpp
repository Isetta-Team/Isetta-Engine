#include <cmath>
#include <limits>
#include "Core/Math/Vector4.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Vector4Test){
  public :

      TEST_METHOD(Magnitude){Math::Vector4 vec{1.f, 1.f, 1.f, 1.f};
Assert::AreEqual(sqrt(4.f), vec.Magnitude());
}  // namespace MathTest

TEST_METHOD(SqrMagnitude) {
  Math::Vector4 vec{1.f, 1.f, 1.f, 1.f};
  Assert::AreEqual(4.f, vec.SqrMagnitude());
}

TEST_METHOD(Normalized) {
  Math::Vector4 vec{5.f, 0.f, 0.f, 0.f};
  Assert::IsTrue(Math::Vector4{1.f, 0.f, 0.f, 0.f} == vec.Normalized());
}

TEST_METHOD(Normalize) {
  Math::Vector4 vec{5.f, 0.f, 0.f, 0.f};
  vec.Normalize();
  Assert::IsTrue(Math::Vector4{1.f, 0.f, 0.f, 0.f} == vec);
}

TEST_METHOD(Equals) {
  Math::Vector4 vec1{5.f, 0.f, 0.f, 0.f};
  Math::Vector4 vec2(5.f + FLT_EPSILON, 0.f, 0.f, 0.f);
  Assert::IsTrue(vec1 == vec2);
}

TEST_METHOD(Dot) {
  Math::Vector4 vec1{5.f, 1.f, 3.f, 2.f};
  Math::Vector4 vec2(2.f, 5.f, 1.f, 6.f);
  Assert::AreEqual(30.f, Math::Vector4::Dot(vec1, vec2));
}

TEST_METHOD(Lerp) {
  Math::Vector4 vec1{1.f, 0.f, 0.f, 2.f};
  Math::Vector4 vec2(0.f, 1.f, 0.f, 0.f);
  Assert::IsTrue(Math::Vector4::Lerp(vec1, vec2, 0.5f) ==
                 Math::Vector4(0.5f, 0.5f, 0.f, 1.f));
}

TEST_METHOD(Distance) {
  Math::Vector4 vec1{1.f, 0.f, 0.f, 0.f};
  Math::Vector4 vec2(0.f, 1.f, 0.f, 3.f);
  Assert::AreEqual(Math::Vector4::Distance(vec1, vec2), sqrt(11.f));
}

TEST_METHOD(Project) {
  Math::Vector4 vec1{1.f, 0.f, 0.f, 0.f};
  Math::Vector4 vec2(0.f, 1.f, 0.f, 0.f);
  Assert::IsTrue(Math::Vector4::Project(vec1, vec2) == Math::Vector4::zero);
}

TEST_METHOD(Scale) {
  Math::Vector4 vec1{1.f, -2.f, 4.f, 1.f};
  Math::Vector4 vec2(5.f, 3.f, 6.f, 10.f);
  Assert::IsTrue(Math::Vector4::Scale(vec1, vec2) ==
                 Math::Vector4(5.f, -6.f, 24.f, 10.f));
}

TEST_METHOD(Slerp) {
  Math::Vector4 vec1{1.f, 0.f, 0.f, 1.f};
  Math::Vector4 vec2(0.f, 1.f, 0.f, 2.f);
  Assert::IsTrue(Math::Vector4::Slerp(vec1, vec2, 0.f) ==
                 Math::Vector4(1.f, 0.f, 0.f, 1.f));
}
}
;
}
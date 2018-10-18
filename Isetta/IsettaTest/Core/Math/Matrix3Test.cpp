#include "Core/Math/Matrix3.h"
#include "Core/Math/Vector3.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(Matrix3Test){public : TEST_METHOD(Determinant){
    Assert::AreEqual(-306.f,
                     Math::Matrix3{6, 1, 1, 4, -2, 5, 2, 8, 7}.Determinant());
}
TEST_METHOD(Get) { Assert::AreEqual(1.f, Math::Matrix3::identity.Get(1, 1)); }
TEST_METHOD(Set) {
  Math::Matrix3 mat{0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f};
  mat.Set(1, 1, 0.f);
  Assert::IsTrue(mat == Math::Matrix3::zero);
}
TEST_METHOD(Inverse) {
  Math::Matrix3 mat{1.f, 2.f, 3.f, 0.f, 1.f, 4.f, 5.f, 6.f, 0.f};
  Math::Matrix3 inv{mat.Inverse()};
  Assert::IsTrue(inv == Math::Matrix3{-24.f, 18.f, 5.f, 20.f, -15.f, -4.f, -5.f,
                                      4.f, 1.f});
}
TEST_METHOD(Transpose) {
  Math::Matrix3 mat{1.f, 2.f, 3.f, 2.f, 4.f, 5.f, 3.f, 5.f, 6.f};
  Assert::IsTrue(mat == mat.Transpose());
}
TEST_METHOD(IsIdentity) {
  Assert::IsTrue(Math::Matrix3::identity.IsIdentity());
  Assert::IsFalse(Math::Matrix3::zero.IsIdentity());
}
TEST_METHOD(IsZero) {
  Assert::IsFalse(Math::Matrix3::identity.IsZero());
  Assert::IsTrue(Math::Matrix3::zero.IsZero());
}
TEST_METHOD(GetRow) {
  Assert::IsTrue(Math::Matrix3::identity.GetRow(0) == Math::Vector3::left);
}
TEST_METHOD(GetCol) {
  Assert::IsTrue(Math::Matrix3::identity.GetCol(0) == Math::Vector3::left);
}
}
;
}
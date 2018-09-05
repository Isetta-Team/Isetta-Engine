#include "Core/Color.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace ColorTest {
TEST_CLASS(ColorTest){public : TEST_METHOD(Addition){Assert::IsTrue(
    Color(0.25f, 0.25f, 0.25f, 1.0f) + Color(0.75f, 0.75f, 0.75f, 0.75f) ==
    Color::white);
}
// TEST_METHOD(Get) { Assert::AreEqual(1.f, Math::Matrix3::identity.Get(1,
// 1)); } TEST_METHOD(Set) {
//  Math::Matrix3 mat{0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f};
//  mat.Set(1, 1, 0.f);
//  Assert::IsTrue(mat == Math::Matrix3::zero);
}
;  // namespace ColorTest
}
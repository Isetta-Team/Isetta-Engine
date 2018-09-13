#include "Core/Color.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace ColorTest {
TEST_CLASS(ColorTest){public : TEST_METHOD(GreyScale){
    Assert::IsTrue(0.5f == Color(0.4f, 0.5f, 0.6f, 1.0f).GreyScale());
}

public:
TEST_METHOD(MaxColorComponent) {
  Assert::IsTrue(0.3f == Color(0.1f, 0.2f, 0.3f, 1.0f).MaxColorComponent());
}

public:
TEST_METHOD(ToString) {
  Assert::IsTrue(std::string("Color(0.1, 0.2, 0.3, 1.0)") ==
                 Color(0.1f, 0.2f, 0.3f, 1.0f).ToString());
}

public:
TEST_METHOD(Lerp) {
  Assert::IsTrue(Color(0.25f, 0.25f, 0.25f, 0.25f) ==
                 Color::Lerp(Color::clear, Color::white, 0.25f));
}
}
;
}  // namespace ColorTest

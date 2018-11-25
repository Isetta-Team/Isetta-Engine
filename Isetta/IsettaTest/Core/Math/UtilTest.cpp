#include <cmath>
#include "Core/Math/Util.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Isetta;

namespace MathTest {
TEST_CLASS(UtilTest){
  public : TEST_METHOD(Abs){Assert::AreEqual(1, Math::Util::Abs(-1));
}
TEST_METHOD(Acos) { Assert::AreEqual(acosf(0.5f), Math::Util::Acos(0.5f)); }
TEST_METHOD(Asin) { Assert::AreEqual(asinf(0.5f), Math::Util::Asin(0.5f)); }
TEST_METHOD(Atan) { Assert::AreEqual(atanf(2.f), Math::Util::Atan(2.f)); }
TEST_METHOD(Atan2) {
  Assert::AreEqual(atan2f(2.f, 2.f), Math::Util::Atan2(2.f, 2.f));
}
TEST_METHOD(Ceil) { Assert::AreEqual(2.f, Math::Util::Ceil(1.8f)); }
TEST_METHOD(CeilToInt) { Assert::AreEqual(2, Math::Util::CeilToInt(1.8f)); }
TEST_METHOD(Clamp) { Assert::AreEqual(1.f, Math::Util::Clamp(0.f, 1.f, 1.5f)); }
TEST_METHOD(ClampInt) { Assert::AreEqual(1, Math::Util::Clamp(0, 1, 3)); }
TEST_METHOD(Clamp01) { Assert::AreEqual(0.f, Math::Util::Clamp01(-1.f)); }
TEST_METHOD(ClosestPowerOfTwo) {
  Assert::AreEqual(32, Math::Util::ClosestPowerOfTwo(30));
}
TEST_METHOD(Cos) { Assert::AreEqual(cosf(3.f), Math::Util::Cos(3.f)); }
TEST_METHOD(Exp) { Assert::AreEqual(1.f, Math::Util::Exp(0.f)); }
TEST_METHOD(Floor) { Assert::AreEqual(1.f, Math::Util::Floor(1.2f)); }
TEST_METHOD(FloorToInt) { Assert::AreEqual(1, Math::Util::FloorToInt(1.2f)); }
TEST_METHOD(InverseLerp) {
  Assert::AreEqual(0.5f, Math::Util::InverseLerp(0.f, 2.f, 1.f));
}
TEST_METHOD(IsPowerOfTwo) {
  Assert::AreEqual(true, Math::Util::IsPowerOfTwo(32));
}
TEST_METHOD(Lerp) { Assert::AreEqual(0.5f, Math::Util::Lerp(0.f, 1.f, 0.5f)); }
TEST_METHOD(Ln) { Assert::AreEqual(logf(5.f), Math::Util::Ln(5.f)); }
TEST_METHOD(Log) { Assert::AreEqual(0.f, Math::Util::Log(1.f, 12.f)); }
TEST_METHOD(Log10) { Assert::AreEqual(2.f, Math::Util::Log10(100.f)); }
TEST_METHOD(Max) {
  Assert::AreEqual(5.f, Math::Util::Max({1.f, 2.f, 3.f, 5.f}));
}
TEST_METHOD(Max_INT) { Assert::AreEqual(5, Math::Util::Max({1, 2, 3, 4, 5})); }
TEST_METHOD(Min) {
  Assert::AreEqual(1.f, Math::Util::Min({1.f, 2.f, 3.f, 5.f}));
}
TEST_METHOD(Min_INT) { Assert::AreEqual(1, Math::Util::Min({1, 2, 3, 4, 5})); }
TEST_METHOD(MoveTowards) {
  Assert::AreEqual(1.f, Math::Util::MoveTowards(0.5f, 1.f, 0.7f));
}
TEST_METHOD(NextPowerOfTwo) {
  Assert::AreEqual(32, Math::Util::NextPowerOfTwo(30));
}
TEST_METHOD(Pow) { Assert::AreEqual(32.f, Math::Util::Pow(2.f, 5.f)); }
TEST_METHOD(Repeat) { Assert::AreEqual(0.5f, Math::Util::Repeat(1.5f, 1.f)); }
TEST_METHOD(Round) { Assert::AreEqual(1.f, Math::Util::Round(0.7f)); }
TEST_METHOD(RoundToInt) { Assert::AreEqual(1, Math::Util::RoundToInt(0.7f)); }
TEST_METHOD(Sign) { Assert::AreEqual(1, Math::Util::Sign(10)); }
TEST_METHOD(Sin) { Assert::AreEqual(sinf(2.f), Math::Util::Sin(2.f)); }
TEST_METHOD(SmoothStep) {
  Assert::AreEqual(0.f, Math::Util::SmoothStep(0.f, 1.f, 0.f));
}
TEST_METHOD(Sqrt) { Assert::AreEqual(2.f, Math::Util::Sqrt(4.f)); }
TEST_METHOD(Square) { Assert::AreEqual(4.f, Math::Util::Square(2.f)); }
TEST_METHOD(Square_Int) { Assert::AreEqual(4, Math::Util::Square(2)); }
TEST_METHOD(Tan) { Assert::AreEqual(tanf(2.f), Math::Util::Tan(2.f)); }
}
;
}  // namespace MathTest
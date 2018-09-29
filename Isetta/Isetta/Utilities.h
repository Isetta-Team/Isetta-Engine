#pragma once
#include <cstdarg>
#include <cstdio>

namespace Isetta {
namespace Utilities {

inline const char* Msg(const char* format, ...) {
  const int MAX_CHARS = 1023;
  static char charBuffer[MAX_CHARS + 1];

  va_list argList;
  va_start(argList, format);
  vsprintf_s(charBuffer, MAX_CHARS, format, argList);
  charBuffer[MAX_CHARS] = '\0';
  va_end(argList);

  return charBuffer;
}

static const unsigned char BitsSetTable256[256] = {
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)};

inline unsigned int CountSetBits(int N) {
  // first chunk of 8 bits from right
  unsigned int cnt =
      BitsSetTable256[N & 0xff] + BitsSetTable256[(N >> 8) & 0xff] +
      BitsSetTable256[(N >> 16) & 0xff] + BitsSetTable256[(N >> 24) & 0xff];
  return cnt;
}

}  // namespace Utilities
}  // namespace Isetta

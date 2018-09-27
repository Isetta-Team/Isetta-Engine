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

}  // namespace Utilities
}  // namespace Isetta

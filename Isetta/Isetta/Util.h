#pragma once
#include <cstdarg>
#include <cstdio>

namespace Isetta {
namespace Util {

inline const char* StrFormat(const char* format, ...) {
  const int charLength = 1024;
  static char charBuffer[charLength];
  sprintf_s(charBuffer, charLength - 1, "");

  va_list argList;
  va_start(argList, format);
  vsprintf_s(charBuffer + strlen(charBuffer), 1023, format, argList);

  return charBuffer;
}

}  // namespace Util
}  // namespace Isetta

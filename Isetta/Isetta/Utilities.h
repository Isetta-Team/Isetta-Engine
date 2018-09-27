#pragma once
#include <cstdarg>
#include <cstdio>

namespace Isetta::Utilities {

inline const char* Msg(const char* format, ...) {
  static char charBuffer[1024];
  sprintf_s(charBuffer, 1023, "");

  va_list argList;
  va_start(argList, format);
  vsprintf_s(charBuffer + strlen(charBuffer), 1023, format, argList);

  return charBuffer;
}

}  // namespace Isetta::Utilities

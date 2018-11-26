#pragma once
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/StopWatch.h"

namespace Isetta::Util {
#define BIND_1(callback, reference) \
  std::bind(&callback, reference, std::placeholders::_1)

inline const char* StrFormat(const char* format, ...) {
  const int MAX_CHARS = 1023;
  static char charBuffer[MAX_CHARS + 1];

  va_list argList;
  va_start(argList, format);
  vsprintf_s(charBuffer, MAX_CHARS, format, argList);
  charBuffer[MAX_CHARS] = '\0';
  va_end(argList);

  return charBuffer;
}

inline int FormatStringV(char* buf, size_t size, const char* format,
                         va_list args) {
  int w = vsnprintf(buf, size, format, args);
  if (buf == NULL) return w;
  if (w == -1 || w >= (int)size) w = (int)size - 1;
  buf[w] = 0;
  return w;
}

inline void StrRemoveSpaces(std::string* str) {
  str->erase(std::remove_if(str->begin(), str->end(), isspace), str->end());
}

inline Array<std::string> StrSplit(const std::string& inStr,
                                   const char separator) {
  if (inStr.empty()) return Array<std::string>{};
  Array<std::string> results;
  Size lastPos = -1;
  Size sepPos = inStr.find(separator);
  while (sepPos != std::string::npos) {
    results.PushBack(inStr.substr(lastPos + 1, sepPos - lastPos - 1));
    lastPos = sepPos;
    sepPos = inStr.find(separator, sepPos + 1);
  }
  results.PushBack(inStr.substr(lastPos + 1, inStr.length() - lastPos - 1));
  return results;
}

inline void Benchmark(const char* name, const Action<>& benchmark) {
  StopWatch stopWatch;
  stopWatch.Start();
  benchmark();
  LOG_INFO(Debug::Channel::General, "[Benchmark] %s took %fs", name,
           stopWatch.EvaluateInSecond());
}

inline float MegaBytesFromBytes(const int byte) {
  return byte / 1024.f / 1024.f;
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

struct UnorderedPairHash {
  template <typename T>
  std::size_t operator()(std::pair<T, T> const& p) const {
    return (std::hash<T>()(p.first) ^ std::hash<T>()(p.second));
  }
  template <typename T>
  bool operator()(const std::pair<T, T>& a, const std::pair<T, T>& b) const {
    if (a.first == b.first && a.second == b.second) return true;
    if (a.first == b.second && a.second == b.first) return true;
    return false;
  }
};
struct PairHash {
  template <typename T, typename S>
  std::size_t operator()(std::pair<T, S> const& p) const {
    return (std::hash<T>()(p.first) ^
            (std::hash<S>()(p.second) << sizeof(p.first)));
  }
  template <typename T, typename S>
  bool operator()(const std::pair<T, S>& a, const std::pair<T, S>& b) const {
    return (a.first == b.first && a.second == b.second);
  }
};

}  // namespace Isetta::Util

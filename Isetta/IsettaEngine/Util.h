#pragma once
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/StopWatch.h"

namespace Isetta::Util {
/**
 * \brief Conveniently bind a member function and pass it into somewhere else as
 * a lambda
 */
#define BIND_1(callback, reference) \
  std::bind(&callback, reference, std::placeholders::_1)

/**
 * \brief Utility to allow printf style string formatting
 */
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

/**
 * \brief Remove white spaces in the input string
 */
inline void StrRemoveSpaces(std::string* str) {
  str->erase(std::remove_if(str->begin(), str->end(), isspace), str->end());
}

/**
 * \brief Split an input string into an array of strings using the input
 * "separator" as separator
 */
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

/**
 * \brief Used to easily benchmark a given procedure and print the result out
 * using the given name
 */
inline void Benchmark(const char* name, const Action<>& benchmark) {
  StopWatch stopWatch;
  stopWatch.Start();
  benchmark();
  LOG_INFO(Debug::Channel::General, "[Benchmark] %s took %fs", name,
           stopWatch.EvaluateInSecond());
}

/**
 * \brief Convert int in bytes to mega bytes
 */
inline float MegaBytesFromBytes(const int byte) {
  return byte / 1024.f / 1024.f;
}

static const unsigned char BitsSetTable256[256] = {
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
    B6(0), B6(1), B6(1), B6(2)};

/**
 * \brief Count the number of bits set (to 1) in the input int
 */
inline unsigned int CountSetBits(int N) {
  // first chunk of 8 bits from right
  unsigned int cnt =
      BitsSetTable256[N & 0xff] + BitsSetTable256[(N >> 8) & 0xff] +
      BitsSetTable256[(N >> 16) & 0xff] + BitsSetTable256[(N >> 24) & 0xff];
  return cnt;
}

/**
 * \brief A hash function that ignores order of the pair. i.e. {A, B} and {B, A}
 * hash to the same thing
 */
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

/**
 * \brief A normal pair hash function
 */
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

} // namespace Isetta::Util

#pragma once
#include <cstddef>
#include <cstdint>

namespace Isetta {
typedef uint8_t Byte;
typedef size_t SizeInt;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef std::ptrdiff_t PtrDiff;
typedef uintptr_t PtrInt;

inline auto operator""_KB(SizeInt const x) {
  return 1024 * x;
}

inline auto operator""_MB(SizeInt const x) {
  return 1024 * 1024 * x;
}

inline auto operator""_GB(SizeInt const x) {
  return 1024 * 1024 * 1024 * x;
}

}  // namespace Isetta

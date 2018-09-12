#pragma once
#include <cstddef>
#include <cstdint>

namespace Isetta {
using Byte = uint8_t;
using SizeInt = size_t;

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using PtrDiff = std::ptrdiff_t;
using PtrInt = uintptr_t;

inline auto operator""_KB(SizeInt const x) { return 1024 * x; }

inline auto operator""_MB(SizeInt const x) { return 1024 * 1024 * x; }

inline auto operator""_GB(SizeInt const x) { return 1024 * 1024 * 1024 * x; }

}  // namespace Isetta

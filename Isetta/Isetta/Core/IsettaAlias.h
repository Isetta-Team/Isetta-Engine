/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>

namespace Isetta {
using Byte = uint8_t;
using Size = size_t;

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

constexpr auto U64_MAX = 0xFFFFFFFFFFFFFFFF;

using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using PtrDiff = std::ptrdiff_t;
using PtrInt = uintptr_t;

using Action = std::function<void()>;

inline auto operator""_KB(Size const x) { return 1024 * x; }

inline auto operator""_MB(Size const x) { return 1024 * 1024 * x; }

inline auto operator""_GB(Size const x) { return 1024 * 1024 * 1024 * x; }

}  // namespace Isetta

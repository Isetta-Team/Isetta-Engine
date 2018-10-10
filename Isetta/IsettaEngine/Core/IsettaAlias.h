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

constexpr U8 U8_MAX = 0xFF;
constexpr U16 U16_MAX = 0xFFFF;
constexpr U32 U32_MAX = 0xFFFFFFFF;
constexpr U64 U64_MAX = 0xFFFFFFFFFFFFFFFF;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using PtrDiff = std::ptrdiff_t;
using PtrInt = uintptr_t;

template <typename... T>
using Action = std::function<void(T...)>;

template <typename result, typename... T>
using Func = std::function<result(T...)>;

inline auto operator""_KB(Size const x) { return 1024 * x; }
inline auto operator""_MB(Size const x) { return 1024 * 1024 * x; }
inline auto operator""_GB(Size const x) { return 1024 * 1024 * 1024 * x; }

}  // namespace Isetta

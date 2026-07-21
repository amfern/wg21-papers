#pragma once

// (C) Copyright 2015 Just Software Solutions Ltd
//
// Distributed under the Boost Software License, Version 1.0.
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or
// organization obtaining a copy of the software and accompanying
// documentation covered by this license (the "Software") to use,
// reproduce, display, distribute, execute, and transmit the
// Software, and to prepare derivative works of the Software, and
// to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire
// statement, including the above license grant, this restriction
// and the following disclaimer, must be included in all copies
// of the Software, in whole or in part, and all derivative works
// of the Software, unless such copies or derivative works are
// solely in the form of machine-executable object code generated
// by a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
// COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE
// LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <type_traits>
#include <utility>
#include <meta>

namespace std {

inline constexpr struct BitmaskType {} bitmask_type;
inline constexpr struct BitmaskRetType {} bitmask_ret_type;

template <typename T>
concept BitmaskTypeLike =
    std::is_enum_v<T> and (!meta::annotations_of_with_type(^^T,meta::type_of(^^bitmask_type)).empty() or
                           requires(T e) { { enable_bitmask_type(e) } -> std::same_as<BitmaskRetType>; });

} // std

template <std::BitmaskTypeLike T>
constexpr auto operator|(T lhs, T rhs) {
  return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

template <std::BitmaskTypeLike T>
constexpr auto operator&(T lhs, T rhs) {
  return static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

template <std::BitmaskTypeLike T>
constexpr auto operator^(T lhs, T rhs) {
  return static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
}

template <std::BitmaskTypeLike T> constexpr auto operator~(T lhs) {
  return static_cast<T>(~std::to_underlying<T>(lhs));
}

template <std::BitmaskTypeLike T>
constexpr T operator|=(T &lhs, T rhs) {
  lhs = lhs | rhs;
  return lhs;
}

template <std::BitmaskTypeLike T>
constexpr T operator&=(T &lhs, T rhs) {
  lhs = lhs & rhs;
  return lhs;
}

template <std::BitmaskTypeLike T>
constexpr T operator^=(T &lhs, T rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

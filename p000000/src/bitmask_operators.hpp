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


// TODO: What do you think about boost?
// Initially were for prototyping new features that are candidates for std
// The direction is now more of a "is useful library", Thread library started in boost. License is really permissive.
// Now we have the https://bemanproject.org/ for prototyping
// The quality of the code is still good

// TODO: I feel strange that i have to implement it it by myslef, is there nothing from the c++ standart library we can use? 
// Draft a proposal to add this to C++, how come such a basic thing is not part of C++ yet?
// Library Evolution group LEWG(do we want to add it to C++) -> LWG(how do we add it?, is the wording right)
// Approval from evolution group
// Get expert in C++ proposal wording
// Official proposal guideline  https://isocpp.org/std/submit-a-proposal
//   probably don't need to float the idea, because it been on the internet for a while
//   Technical Specifications is the precise library wording, it doesn't need to be in the initial proposal
//   post meeting https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/#mailing2026-04
//   Disscuss via zoom with the commiette

// template https://github.com/mpark/wg21
// example paper https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3411r5.html


// binary operators are not defined by default on enum class, solution is here https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html
// https://stackoverflow.com/questions/45541760/c-enum-flags-vs-bitset
// https://stackoverflow.com/questions/42481154/how-to-use-bitmask-operators-hpp-with-namespace-and-classes
// https://github.com/Dalzhim/ArticleEnumClass-v2/blob/master/Readme.md
// Adapt the original solution to C++23 with concepts https://andreasfertig.com/blog/2024/01/cpp20-concepts-applied/
// reflection example example paper https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3411r5.html

#include <type_traits>
#include <utility>
// #include <experimental/meta>
#include <meta>

#include "src/p3394.hpp"

template <auto V> struct Derive { };
template <auto V> inline constexpr Derive<V> derive;

inline constexpr struct{} Bitmask;

template<typename T>
concept BitmaskLike = std::is_enum_v<T> and std::meta::has_annotation(^^T, derive<Bitmask>);

template<BitmaskLike T>
constexpr auto
operator|(const T lhs, const T rhs)
{
  return static_cast<T>(std::to_underlying(lhs) |
                        std::to_underlying(rhs));
}

template<typename T>
constexpr auto
operator&(const T lhs, const T rhs)
{
  return static_cast<T>(std::to_underlying(lhs) &
                        std::to_underlying(rhs));
}

template<typename T> 
constexpr auto
operator^(const T lhs, const T rhs)
{
  return static_cast<T>(std::to_underlying(lhs) ^
                        std::to_underlying(rhs));
}


template<typename T>
constexpr auto
operator~(const T lhs)
{
  return static_cast<T>(~std::to_underlying(lhs));
}


template<typename T>
constexpr auto
operator|=(const T &lhs, const T rhs)
{
  lhs = static_cast<T>(std::to_underlying(lhs) |
                        std::to_underlying(rhs));
  return lhs;
}

template<typename T>
requires(std::is_enum_v<T>and requires(T e) {
  enable_bitmask_operators(e);
}) constexpr auto
operator&=(const T &lhs, const T rhs)
{
  lhs = static_cast<T>(std::to_underlying(lhs) &
                        std::to_underlying(rhs));
  return lhs;
}


template<typename T>
constexpr auto
operator^=(T &lhs, const T rhs)
{
  lhs = static_cast<T>(std::to_underlying(lhs) ^
                       std::to_underlying(rhs));
  return lhs;
}

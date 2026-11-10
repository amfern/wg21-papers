#pragma once

#include <type_traits>
#include <utility>
#include <meta>

// TODO: add rules_lint to lint everything properrly

namespace std {

// inline constexpr struct BitmaskType {} bitmask_type;

// template <typename T>
// concept BitmaskTypeLike =
//     std::is_enum_v<T> and (!meta::annotations_of_with_type(^^T,meta::type_of(^^bitmask_type)).empty() or
//                            requires(T e) { { enable_bitmask_type(e) } -> std::same_as<BitmaskRetType>; });

template <typename E>
class BitmaskWrapper {
public:
  E value;

  friend constexpr auto operator|(BitmaskWrapper<E> lhs, BitmaskWrapper<E> rhs) {
    E val = static_cast<E>(std::to_underlying(lhs.value) | std::to_underlying(rhs.value));
    return BitmaskWrapper<E>{val};
  }

  friend constexpr auto operator&(BitmaskWrapper<E> lhs, BitmaskWrapper<E> rhs) {
    E val = static_cast<E>(std::to_underlying(lhs) & std::to_underlying(rhs));
    return BitmaskWrapper<E>{val};
  }

  friend constexpr auto operator^(BitmaskWrapper<E> lhs, BitmaskWrapper<E> rhs) {
    E val = static_cast<E>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
    return BitmaskWrapper<E>{val};
  }

  friend constexpr auto operator~(BitmaskWrapper<E> lhs) {
    // we treat boolean specially because of integral promotion rules
    // https://sub.fail/posts/complement-of-true/
    E val; 
    if constexpr (std::same_as<bool, std::underlying_type_t<E>>) {
      val = static_cast<E>(!static_cast<bool>(lhs.value));
    } else {
      val = static_cast<E>(~std::to_underlying(lhs.value));
    }

    return BitmaskWrapper<E>{val};
  }

  friend constexpr BitmaskWrapper<E> operator|=(BitmaskWrapper<E> &lhs, BitmaskWrapper<E> rhs) {
    lhs = lhs | rhs;
    return lhs;
  }

  friend constexpr BitmaskWrapper<E> operator&=(BitmaskWrapper<E> &lhs, BitmaskWrapper<E> rhs) {
    lhs = lhs & rhs;
    return lhs;
  }

  friend constexpr BitmaskWrapper<E> operator^=(BitmaskWrapper<E> &lhs, BitmaskWrapper<E> rhs) {
    lhs = lhs ^ rhs;
    return lhs;
  }

  friend constexpr bool operator==(BitmaskWrapper<E> lhs, BitmaskWrapper<E> rhs) {
    return lhs.value == rhs.value;
  }
};

}

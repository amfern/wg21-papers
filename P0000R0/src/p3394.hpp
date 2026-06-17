#pragma once


#include <type_traits>
#include <utility>
#include <meta>

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3394r4.html
namespace std::meta {
namespace __impl {
  template<auto... vals>
  struct replicator_type {
    template<typename F>
      constexpr void operator>>(F body) const {
        (body.template operator()<vals>(), ...);
      }
  };

  template<auto... vals>
  replicator_type<vals...> replicator = {};
}

template<typename R>
consteval auto expand(R range) {
  std::vector<std::meta::info> args;
  for (auto r : range) {
    args.push_back(std::meta::reflect_constant(r));
  }
  return substitute(^^__impl::replicator, args);
}

template <typename T>
consteval auto annotation_of(info r) -> optional<T> {
    optional<info> res;
    for (info a : annotations_of(r)) {
        if (type_of(a) != ^^T) {
            continue;
        }

        if (res.has_value() and *res != a) {
            throw "oops";
        }

        res = a;
    }
    return res.transform([](info r){ return extract<T>(r); });
}

template <typename T>
consteval auto has_annotation(info r, T const& value) -> bool {
    auto expected = reflect_constant(value);
    for (info a : annotations_of(r)) {
        if (constant_of(a) == expected) {
            return true;
        }
    }
    return false;
}
}

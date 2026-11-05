---
title: "Bitmask operations for enums"
document: P4313R2
date: today
audience: LWG
author:
  - name: Iliya Guterman
    email: <iliyagutermann@gmail.com>
  - name: Anthony Williams
    email: <anthony@justsoftwaresolutions.co.uk>
toc: true
status: progress
tag: enum
---

# Revision history {- .unlisted .collapsed}
R2:

- address concern over bool underlying type raised in https://dryperspective.github.io/posts/complement-of-true/
- Depends on the proposal [@P4380R1]{.title}, to address name lookup issues raised in https://brevzin.github.io/c++/2026/10/14/push-vs-pull-ii/, and adopt the solution from there.

R1:

- Fix operator precedence in the opening example
- Demonstrate slightly complicated case in the opening example

R0:

- Original version

# Proposal

We propose enabling bitmask operators for enum classes that have a `std::bitmask_type` annotation, to provide similar behavior to [bitmask.types]{.sref}.
```C++
enum class [[=std::bitmask_type]] Permission {
  None    = 0,
  Read    = 1 << 0,
  Write   = 1 << 1,
  Execute = 1 << 2,
};

int main () {
    // Combine permissions
    Permission user_perms = Permission::Read | Permission::Write | Permission::Execute;

    if ((user_perms & Permission::Read) == Permission::Read)
        std::cout << "Can read";

    Permission can_read_and_write = Permission::Read | Permission::Write;
    if ((user_perms & can_read_and_write) == can_read_and_write)
        std::cout << "Can read and write";
}
```

# Motivations

## Motivation 1: Bitmask operators on enum was something that used to work well in C
Bitmask operators on enum was something that used to work well in C, and we lost it with `enum class` even though enum class provided us with the very type safety that made this a safer thing to do.
it's also unclear why bitmask operators were omitted in the first place. They aren't even mentioned in the proposal [n2347](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2347.pdf)
       
```C
typedef enum Permission {
    PERMISSION_NONE    = 0,
    PERMISSION_READ    = 1u << 0, // 0b001
    PERMISSION_WRITE   = 1u << 1, // 0b010
    PERMISSION_EXECUTE = 1u << 2, // 0b100
} Permission;

int main(void)
{
    Permission user_perms = PERMISSION_READ | PERMISSION_WRITE;

    // Check permissions
    if (user_perms & PERMISSION_READ) {
        printf("Can read\n");
    }
}
```

## Motivation 2: Drawing inspiration from other programming languages.

As a developer coming from languages other than C++, I grew accustomed to using enums to represent flags.
However, such behavior is noticeably absent from the C++ language.

### Bitmask in: Python
```python
from enum import IntFlag

class Permission(IntFlag):
    READ    = 1 << 0  # 0b001
    WRITE   = 1 << 1  # 0b010
    EXECUTE = 1 << 2  # 0b100

# Combine permissions
user_perms = Permission.READ | Permission.WRITE

# Check permissions
if user_perms & Permission.READ:
    print("Can read")
```

### Bitmask in: C\#
```C#
using System;

[Flags]
public enum Permission
{
    None    = 0,
    Read    = 1 << 0, // 0b001
    Write   = 1 << 1, // 0b010
    Execute = 1 << 2, // 0b100
}

class Program
{
    static void Main()
    {
        // Combine permissions
        Permission userPerms = Permission.Read | Permission.Write;

        Console.WriteLine(userPerms);
        // Read, Write

        // Check permissions
        if ((userPerms & Permission.Read) != Permission.None) {
            Console.WriteLine("Can read");
        }
    }
}
```

### Bitmask in: Rust
```Rust
// Cargo.toml: bitflags = "2.4" (or latest)
use bitflags::bitflags;

bitflags! {
    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    pub struct Permissions: u32 {
        const READ    = 0b00000001;
        const WRITE   = 0b00000010;
        const EXECUTE = 0b00000100;
    }
}

fn main() {
    // Combine flags using bitwise OR
    let user_perms = Permissions::READ | Permissions::WRITE;

    // Check permissions
    if user_perms.contains(Permissions::READ) {
        println!("Can read");
    }
}
```

### Bitmask in: JavaScript
```Javascript
const Permission = {
    READ:    1 << 0, // 0b001
    WRITE:   1 << 1, // 0b010
    EXECUTE: 1 << 2, // 0b100
};

// Combine permissions
let userPerms = Permission.READ | Permission.WRITE;

// Check permissions
if (userPerms & Permission.READ) {
    console.log("Can read");
}
```

## Motivation 3: Reduce boilerplate code

A repeated boilerplate code for bitmask behavior can be found with quite high frequency in the wild

For example, in LLVM project, the same boilerplate code is repeated multiple times.
- [perms.h](https://github.com/llvm/llvm-project/blob/main/libcxx/include/__filesystem/perms.h)
- [byte.h](https://github.com/llvm/llvm-project/blob/main/libcxx/include/__cstddef/byte.h)
- [perm_options.h](https://github.com/llvm/llvm-project/blob/main/libcxx/include/__filesystem/perm_options.h)

```C++
enum class perms : unsigned {
  none = 0,

  owner_read  = 0400,
  owner_write = 0200,
  owner_exec  = 0100,
  owner_all   = 0700,
  ...
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline constexpr perms operator&(perms __lhs, perms __rhs) {
  return static_cast<perms>(static_cast<unsigned>(__lhs) & static_cast<unsigned>(__rhs));
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline constexpr perms operator|(perms __lhs, perms __rhs) {
  return static_cast<perms>(static_cast<unsigned>(__lhs) | static_cast<unsigned>(__rhs));
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline constexpr perms operator^(perms __lhs, perms __rhs) {
  return static_cast<perms>(static_cast<unsigned>(__lhs) ^ static_cast<unsigned>(__rhs));
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline constexpr perms operator~(perms __lhs) {
  return static_cast<perms>(~static_cast<unsigned>(__lhs));
}

_LIBCPP_HIDE_FROM_ABI inline perms& operator&=(perms& __lhs, perms __rhs) { return __lhs = __lhs & __rhs; }

_LIBCPP_HIDE_FROM_ABI inline perms& operator|=(perms& __lhs, perms __rhs) { return __lhs = __lhs | __rhs; }

_LIBCPP_HIDE_FROM_ABI inline perms& operator^=(perms& __lhs, perms __rhs) { return __lhs = __lhs ^ __rhs; }

_LIBCPP_END_NAMESPACE_FILESYSTEM
```

The above can be replaced with the following:
```
enum class [[=std::bitmask_type]] perms : unsigned {
  none = 0,

  owner_read  = 0400,
  owner_write = 0200,
  owner_exec  = 0100,
  owner_all   = 0700,
  ...
};
```

## Motivation 4: Standardize solution under C++ standard library
This type of use case is a sought-after feature and has led to the development of many standalone solutions.

1. A bitmask solution was proposed by Anthony Williams in 2015: [Using Enum Classes as Bitfields](https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html). 
1. Later on in 2024 Evolution to Anthony's proposal with C++20 concepts: [C++20 Concepts applied - Safe bitmasks using scoped enums](https://andreasfertig.com/blog/2024/01/cpp20-concepts-applied/)
1. Standalone Header Library for bitmask behavior: [Bitmask operators and typesafe comparisons for enum class](https://github.com/Dalzhim/ArticleEnumClass-v2)
1. Bit Flag Values [Enumerations in C++](https://dev.to/pauljlucas/enumerations-in-c-pn9)
1. Stack-overflow questions from developers looking for bitmask behavior from `enum class`
	1. [C++ enum flags vs bitset](https://stackoverflow.com/questions/45541760/c-enum-flags-vs-bitset)
	1. [How to use bitmask_operators.hpp with namespace and classes](https://stackoverflow.com/questions/42481154/how-to-use-bitmask-operators-hpp-with-namespace-and-classes)
1. Related discussion from reddit: [Am I the only one who thinks this? (about enum)](https://www.reddit.com/r/cpp/s/n5UCCufvBc)

# Considered alternatives solutions

## Alternatives: Can I use std::bitset instead?

While std::bitset supports bitwise operations, it is limited in other respects.
 
### std::bitset does not offer same ergonomics as enum class
std::bitset requires the user to provide meaningful names, for example:
```C++
#include <bit>
#include <bitset>
#include <cassert>
#include <iostream>
#include <stdexcept>

class BitSetA : public std::bitset<8> {
public:
    static constexpr std::size_t write_flag = 2;

    // Users must create meaningful function names or additional abstractions to represent the individual bit names
    bool is_write_enabled() {
        return test(write_flag);
    }

    void toggle_write_enabled() {
        flip(write_flag);
    }
};

int main () {
    BitSetA a;
    std::cout << a.is_write_enabled() << std::endl;
    a.toggle_write_enabled();
    std::cout << a.is_write_enabled() << std::endl;
}
```

### std::bitset does not provide the same type safety
`std::bitset` also does not provide the same type-safety guarantees as `enum class`. 
The resulting type remains the generic `std::bitset` type, so the original semantic type information is lost during operations.

See for example this StackOverflow question: [C++ enum flags vs bitset](https://stackoverflow.com/questions/45541760/c-enum-flags-vs-bitset)
```C++
#include <bit>
#include <bitset>
#include <cassert>
#include <iostream>
#include <stdexcept>

class BitSetA : public std::bitset<8> { };
class BitSetB : public std::bitset<8> { }

int main () {
    BitSetA a;
    BitSetB b;
    
    // a and b decay into std::bitset and lose their type safety
    std::bitset<8> c = a & b;
}
```

# Design

This proposal builds on the example from [@P4380R1]{.title} by Barry Revzin using `queue_injection()` to add the bitmask operators to any `enum class` with the appropriate annotation. 

## Design consideration
1. No implicit conversion: It should not be possible to assign bitmasks of different types to each other or perform operations between them.
2. User should be able to use bitwise operation normally as they would with plain C-Style `int` 
3. Enabling bitmask behavior([bitmask.types]{.sref}) on `enum class` is easy and simple for the user.

# Reference Implementation
1. Godbolt using Barry's clang [https://godbolt.org/z/TbYsv4sPx](https://godbolt.org/z/TbYsv4sPx)
<!-- TODO: $ operator has red syntax because it's not yet standart C++ operator  --> 
```{.cpp em=none}
#include <type_traits>
#include <utility>
#include <meta>

namespace std {
    struct BitmaskType {
        static consteval auto on_complete(std::meta::info ty) -> void {
            if (!is_scoped_enum_type(ty)) {
               // we support only for enum class type
               return;
            }

            std::meta::operators op_for_tenary =
                is_same_type(std::meta::underlying_type(ty), ^^bool)
                    ? std::meta::operators::op_exclamation
                    : std::meta::operators::op_tilde;

            queue_injection(^^{
                constexpr $ty operator~($ty e) {
                    return static_cast<$ty>($op_for_tenary std::to_underlying(e));
                }
            });

            auto binary = [=](std::meta::operators op, std::meta::operators op_eq) {
                queue_injection(^^{
                    constexpr $ty operator $op($ty lhs, $ty rhs) {
                        return static_cast<$ty>(std::to_underlying(lhs) $op std::to_underlying(rhs));
                    }

                    constexpr ${ty}& operator $op_eq(${ty}& lhs, $ty rhs) {
                        lhs = lhs $op rhs;
                        return lhs;
                    }
                });
            };

            using enum std::meta::operators;
            binary(op_pipe,      op_pipe_equals);
            binary(op_ampersand, op_ampersand_equals);
            binary(op_caret,     op_caret_equals);
        }
    };

    inline constexpr auto bitmask_type = BitmaskType();
} // std
```

# Wording
::: wording

## Add `<bitmask>` header

Add new header `<bitmask>` to table Table 24: C++ library [headers]{.sref}

## Add a new sections to chapter [utilities]{.sref} after [bitset]{.sref}
With the content:

::: add
> # 22.x.1 Header `<bitmask>` synopsis {-}
> The header `<bitmask>` defines token sequence injection, which define all bitmask operator function for [bitmask.types]{.sref}
> ```cpp
> namespace std {
> // 22.x.2, bitmask struct for operator injection
> struct BitmaskType { see below };   // exposition only
>
> // 22.x.3, annotation type
> inline constexpr BitmaskType bitmask_type = unspecified;
>
> ```
> 
> # 22.x.2 Annotation `struct BitmaskType` {-}
> [1]{.pnum} When a definition of a scoped enumeration type T with an annotation of type BitmaskType becomes complete, then for each operator @ in (|,^,&), definitions of that operator and the corresponding compound assignment operator are injected into the scoped enumeration definition as follows: 
> ```cpp
> constexpr T operator@ (T lhs,T rhs) noexcept {
>   return static_cast<T>(to_underlying(lhs) @ to_underlying(rhs));
> }
> 
> constexpr T& operator@= (T &lhs,T rhs) noexcept {
>   lhs = lhs @ rhs; return lhs;
> }
> ```
> [2]{.pnum} A definition of the complement operator is also injected into the scope of enumeration definition as follows:
> For Boolean type:
> ```cpp
> constexpr T operator~ (T lhs) noexcept {
>    return bool(lhs);
> } 
> ```
> For every other type:
> ```cpp
> constexpr T operator~ (T lhs) noexcept {
>    return static_cast<T>(~to_underlying(lhs));
> } 
> ```

> # 22.x.3 Annotation `bitmask_type` {-}
> ```cpp
> namespace std {
> 	inline constexpr BitmaskType bitmask_type;
> }
> ```
> [1]{.pnum} bitmask_type is an instance of a unique unspecified type that is suitable for use as an annotation ([dcl.attr.annotation]{.sref}).
>
> [1]{.pnum}
> 
> ::: example
> ```cpp
> enum class [[=std::bitmask_type]] Permission {
> 	None    = 0,
> 	read    = 1 << 0,
> 	Write   = 1 << 1,
> 	Execute = 1 << 2,
> };
> ```
> The enum `Permission` will now have appropriate bitmask operators
> :::
>
:::

:::

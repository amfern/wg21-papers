// main.cpp
#include <cassert>

#include "src/bitmask.hpp"

enum class [[=std::bitmask_type]] A { x = 1, y = 2 };

enum class B : unsigned long { x = 0x80000000, y = 0x40000000 };
constexpr std::BitmaskRetType enable_bitmask_type(B);

enum class C{x,y};

int main() {
    A a1=A::x | A::y;
    A a2=a1&A::y; // Turn on A::y only
    assert(A::y == a2);
    a2^=A::x; // toggle A::x
    // can i change the operator precedence
    // warning: | has lower precedence than ==; == will be evaluated first [-Wparentheses]
    assert((A::x | A::y) == a2);
    A a3=a1&~A::x; // Turn off A::X
    assert(A::y == a3);

    B b1=B::x | B::y;
    B b2=b1&B::y;
    assert(B::y == b2);
    b2^=B::x;
    assert((B::x | B::y) == b2);
    B b3=b1&~B::x;
    assert(B::y == b3);
}


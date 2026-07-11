#include "src/bitmask.hpp"

#include "gtest/gtest.h"


// enable bitmask operators on A class
enum class [[=std::bitmask_type]] A { x = 1, y = 2 };

TEST(BitmaskOperators, All) {
    A a1=A::x | A::y;
    A a2=a1&A::y; // Turn on A::y only
    ASSERT_EQ(A::y, a2);
    a2^=A::x; // toggle A::x
    ASSERT_EQ(A::x | A::y, a2);
    A a3=a1&~A::x; // Turn off A::X
    ASSERT_EQ(A::y, a3);
    a3|= A::x; // turn on A::x
    ASSERT_EQ(A::x | A::y, a3);
    a3&= A::x; // turn on A::x only
    ASSERT_EQ(A::x, a3);
}

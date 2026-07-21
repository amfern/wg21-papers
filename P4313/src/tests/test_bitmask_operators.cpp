#include "src/bitmask.hpp"

#include "gtest/gtest.h"


// enable bitmask operators on A class
enum class [[=std::bitmask_type]] A { x = 1, y = 2, z = 4 };

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

    A a4=A::x | A::y | A::z;

    ASSERT_NE(a4, a1);
    a4^=A::z;
    ASSERT_EQ(a4, a1);
}

enum class [[=std::bitmask_type]] Permission {
  None    = 0,
  Read    = 1 << 0,
  Write   = 1 << 1,
  Execute = 1 << 2,
};

TEST(BitmaskOperators, example_from_paper) {
    // Combine permissions
    Permission user_perms = Permission::Read | Permission::Write | Permission::Execute;
    Permission can_read_and_write = Permission::Read | Permission::Write;

    ASSERT_TRUE((user_perms & Permission::Read) == Permission::Read);
    ASSERT_TRUE((user_perms & can_read_and_write) == can_read_and_write);
}

TEST(BitmaskOperators, static_assert) {
    // Combine permissions
    constexpr Permission user_perms = Permission::Read | Permission::Write | Permission::Execute;
    constexpr Permission can_read_and_write = Permission::Read | Permission::Write;

    static_assert(constexpr (user_perms & Permission::Read) == Permission::Read);
    static_assert(constexpr (user_perms & can_read_and_write) == can_read_and_write);
}

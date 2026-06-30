#include <filesystem>

#include "src/bitmask.hpp"

#include "gtest/gtest.h"

// TODO: i must include  use it for the reference to get caught
using namespace std;

enum class B : unsigned long { x = 0x80000000, y = 0x40000000 };

// enable bitmask operators on B class, can be used on predefined enums
constexpr std::BitmaskRetType enable_bitmask_type(B);

TEST(BitmaskOperatorsPredefinedEnum, All) {
    B b1=B::x | B::y;
    B b2=b1&B::y;
    ASSERT_EQ(B::y, b2);
    b2^=B::x;
    ASSERT_EQ(B::x | B::y, b2);
    B b3=b1&~B::x;
    ASSERT_EQ(B::y, b3);
}

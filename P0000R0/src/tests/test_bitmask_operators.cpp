#include "src/bitmask.hpp"

#include "gtest/gtest.h"

// TODO: i must include  use it for the reference to get caught
using namespace std;

// enable bitmask operators on A class
enum class [[=std::bitmask_type]] A { x = 1, y = 2 };

enum class C{x,y};

TEST(BitmaskOperators, All) {
    A a1=A::x | A::y;
    A a2=a1&A::y; // Turn on A::y only
    ASSERT_EQ(A::y, a2);
    a2^=A::x; // toggle A::x
    ASSERT_EQ(A::x | A::y, a2);
    A a3=a1&~A::x; // Turn off A::X
    ASSERT_EQ(A::y, a3);
}


// TODO: how to test for this thing shouldn't work?
//            how to test we get a compile error?
//            Look in arene-base, use the build system and expect error return code and grep for error
// TEST(LedBitmaskOperators, ShouldFailToBuild) {
//     C c1=C::x | C::y;
//     C c2=c1&C::y;
//     c2^=C::x;
//     C c3=~c1;
// }

#include "src/bitmask.hpp"

enum class [[=std::bitmask_type]] B{x,y};
enum class [[=std::bitmask_type]] C{x,y};

C c1 = C::x | C::y | B::x | B::y;
C c2 = C::x & C::y & B::x & B::y;
C c3 = C::x ^ C::y ^ B::x ^ B::y;
C c4 = ~(B::x&~B::y);

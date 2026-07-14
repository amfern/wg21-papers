enum class C{x,y};

C c1 = C::x | C::y;
C c2 = c1 & C::y;
c2 ^= C::x;
C c3 = ~c1;

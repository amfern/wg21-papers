#include "src/bitmask.hpp"

namespace N {
  enum class[[=std::bitmask_type]] Permission {
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    Execute = 1 << 2,
  };
}

namespace M {
    // some unrelated type
    struct Mask { };
    auto operator|(Mask, Mask) -> Mask;

    auto something(N::Permission permissions) -> void {
        using enum N::Permission;
        if ((permissions | Write) == Write) {
            // do writing
        }
    }
}

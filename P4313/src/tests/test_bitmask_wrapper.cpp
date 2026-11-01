#include "src/bitmask_wrapper.hpp"

// #include "gtest/gtest.h"

namespace N {
  enum class Permission {
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    Execute = 1 << 2,
  };
  using PermissionBitmask = std::BitmaskWrapper<Permission>;

  struct Mask { };
  auto operator|(Mask, Mask) -> Mask;

  auto something(N::PermissionBitmask permissions) -> void {
    using enum N::Permission;

    if ((permissions | N::PermissionBitmask{Write}) == N::PermissionBitmask{Write}) {
      // do writing
    }
  }
};

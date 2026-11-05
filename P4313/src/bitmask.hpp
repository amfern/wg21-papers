#pragma once

#include <type_traits>
#include <utility>
#include <meta>

namespace std {
    // TODO(ask): if the lookup is an issue, it possiblie to do some compiler change, and add a new type of enums that have bitmask operators? eg `bitmask class {}`
    // TODO(ask): can we inherite enum eg class enum A : enum B? why not , i thik it should be available also in C++ for extending enums.
    // TODO: validate that the enum flags values are not colliding and can be used as flags
    struct BitmaskType {
        static consteval auto on_complete(std::meta::info ty) -> void {
            if (!is_scoped_enum_type(ty)) {
               // TODO: maybe i should add an error message here, and test the error message
               // we support only for enum class type
               return;
            }

            // TODO(ask): maybe the sensible thing to do is not allow bool as underlying type?
            std::meta::operators tenary_op_to_use =
                is_same_type(std::meta::underlying_type(ty), ^^bool)
                    ? std::meta::operators::op_exclamation
                    : std::meta::operators::op_tilde;

            queue_injection(^^{
                constexpr $ty operator~($ty e) {
                    return static_cast<$ty>($tenary_op_to_use std::to_underlying(e));
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

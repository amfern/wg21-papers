#pragma once

// (C) Copyright 2015 Just Software Solutions Ltd
//
// Distributed under the Boost Software License, Version 1.0.
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or
// organization obtaining a copy of the software and accompanying
// documentation covered by this license (the "Software") to use,
// reproduce, display, distribute, execute, and transmit the
// Software, and to prepare derivative works of the Software, and
// to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire
// statement, including the above license grant, this restriction
// and the following disclaimer, must be included in all copies
// of the Software, in whole or in part, and all derivative works
// of the Software, unless such copies or derivative works are
// solely in the form of machine-executable object code generated
// by a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
// COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE
// LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <type_traits>
#include <utility>
#include <meta>

namespace std {

inline constexpr struct BitmaskType {} bitmask_type;
inline constexpr struct BitmaskRetType {} bitmask_ret_type;

template <typename T>
concept BitmaskTypeLike =
    std::is_enum_v<T> and (!meta::annotations_of_with_type(^^T,meta::type_of(^^bitmask_type)).empty() or
                           requires(T e) { { enable_bitmask_type(e) } -> std::same_as<BitmaskRetType>; });

} // std

/**
 * Wrapper for an enumerator that provides implicit bool conversion
 */
template <typename T>
struct enumerator
{
	constexpr enumerator(const T& value) : value(value) {}
	constexpr explicit operator bool() const
	{
		return std::to_underlying(value) != 0;
	}
	constexpr operator T() const
	{
		return value;
	}
	
	T value;
};

/**
 * Wrapper that differentiates combined enumerators from a single enumerator
 * to provent accidental comparisons between a bitmask and a single enumerator
 * using operator== or operator!=
 */
template <typename T>
struct bitmask
{
	using underlying_type = typename std::underlying_type<T>::type;

	constexpr bitmask(const T& value) : value(std::to_underlying(value)) {}
	constexpr bitmask(const enumerator<T>& enumerator) : value(std::to_underlying(enumerator.value)) {}
	constexpr explicit operator bool() const
	{
		return value != 0;
	}
	
	underlying_type value;
};

// template <std::BitmaskTypeLike T>
// constexpr auto operator|(T lhs, T rhs) {
//   return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
// }

/**
 * operator|(T, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const T& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs))};
}

/**
 * operator|(enumerator<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const enumerator<T>& lhs, const enumerator<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) | std::to_underlying(rhs.value))};
}

/**
 * operator|(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const bitmask<T>& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value | rhs.value)};
}

/**
 * operator|(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const bitmask<T>& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value | std::to_underlying(rhs))};
}

/**
 * operator|(T, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const T& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs) | rhs.value)};
}

/**
 * operator|(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const bitmask<T>& lhs, const enumerator<T>& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value | std::to_underlying(rhs.value))};
}

/**
 * operator|(enumerator<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const enumerator<T>& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) | rhs.value)};
}

/**
 * operator|(enumerator<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const enumerator<T>& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) | std::to_underlying(rhs))};
}

/**
 * operator|(T, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|(const T& lhs, const enumerator<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs.value))};
}


/**
 * operator&(T, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const T& lhs, const T& rhs)
{
    // TODO(ask): why is this assert needed?
	assert((std::to_underlying(lhs) & (std::to_underlying(lhs) - 1)) == 0);
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs))};
}

/**
 * operator&(enumerator<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const enumerator<T>& lhs, const enumerator<T>& rhs)
{
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs.value) & std::to_underlying(rhs.value))};
}

/**
 * operator&(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const bitmask<T>& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value & rhs.value)};
}

/**
 * operator&(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const bitmask<T>& lhs, const T& rhs)
{
	return enumerator<T>{static_cast<T>(lhs.value & std::to_underlying(rhs))};
}

/**
 * operator&(T, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const T& lhs, const bitmask<T>& rhs)
{
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs) & rhs.value)};
}

/**
 * operator&(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const bitmask<T>& lhs, const enumerator<T>& rhs)
{
	return enumerator<T>{static_cast<T>(lhs.value & std::to_underlying(rhs.value))};
}

/**
 * operator&(enumerator<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const enumerator<T>& lhs, const bitmask<T>& rhs)
{
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs.value) & rhs.value)};
}

/**
 * operator&(T, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const T& lhs, const enumerator<T>& rhs)
{
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs.value))};
}

/**
 * operator&(enumerator<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&(const enumerator<T>& lhs, const T& rhs)
{
	return enumerator<T>{static_cast<T>(std::to_underlying(lhs.value) & std::to_underlying(rhs))};
}


/**
 * operator^(T, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const T& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs))};
}

/**
 * operator^(enumerator<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const enumerator<T>& lhs, const enumerator<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) ^ std::to_underlying(rhs.value))};
}

/**
 * operator^(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const bitmask<T>& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value ^ rhs.value)};
}

/**
 * operator^(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const bitmask<T>& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value ^ std::to_underlying(rhs))};
}

/**
 * operator^(T, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const T& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs) ^ rhs.value)};
}

/**
 * operator^(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const bitmask<T>& lhs, const enumerator<T>& rhs)
{
	return bitmask<T>{static_cast<T>(lhs.value ^ std::to_underlying(rhs.value))};
}

/**
 * operator^(enumerator<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const enumerator<T>& lhs, const bitmask<T>& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) ^ rhs.value)};
}

/**
 * operator^(enumerator<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^(const enumerator<T>& lhs, const T& rhs)
{
	return bitmask<T>{static_cast<T>(std::to_underlying(lhs.value) ^ std::to_underlying(rhs))};
}

/**
 * operator~(T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator~(const T& value)
{
	return bitmask<T>{static_cast<T>(~ std::to_underlying(value))};
}

/**
 * operator~(enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator~(const enumerator<T>& lhs)
{
	return bitmask<T>{static_cast<T>(~ std::to_underlying(lhs.value))};
}

/**
 * operator&=(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&=(bitmask<T>& lhs, const T& rhs)
{
	lhs.value &= std::to_underlying(rhs);
	return lhs;
}

/**
 * operator&=(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&=(bitmask<T>& lhs, const enumerator<T>& rhs)
{
	lhs.value &= std::to_underlying(rhs.value);
	return lhs;
}

/**
 * operator&=(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator&=(bitmask<T>& lhs, const bitmask<T>& rhs)
{
	lhs.value &= rhs.value;
	return lhs;
}

/**
 * operator|=(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|=(bitmask<T>& lhs, const T& rhs)
{
	lhs.value |= std::to_underlying(rhs);
	return lhs;
}

/**
 * operator|=(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|=(bitmask<T>& lhs, const enumerator<T>& rhs)
{
	lhs.value |= std::to_underlying(rhs.value);
	return lhs;
}

/**
 * operator|=(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator|=(bitmask<T>& lhs, const bitmask<T>& rhs)
{
	lhs.value |= rhs.value;
	return lhs;
}

/**
 * operator^=(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^=(bitmask<T>& lhs, const T& rhs)
{
	lhs.value ^= std::to_underlying(rhs);
	return lhs;
}

/**
 * operator^=(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^=(bitmask<T>& lhs, const enumerator<T>& rhs)
{
	lhs.value ^= std::to_underlying(rhs.value);
	return lhs;
}

/**
 * operator^=(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator^=(bitmask<T>& lhs, const bitmask<T>& rhs)
{
	lhs.value ^= rhs.value;
	return lhs;
}



/**
 * operator==(enumerator<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
// TODO: should i return enumerator<T> type here?
constexpr auto operator==(const enumerator<T>& lhs, const enumerator<T>& rhs)
{
	return lhs.value == rhs.value;
}

/**
 * operator==(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const bitmask<T>& lhs, const bitmask<T>& rhs)
{
	return lhs.value == rhs.value;
}

/**
 * operator==(enumerator<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const enumerator<T>& lhs, const T& rhs)
{
	return lhs.value == rhs;
}

/**
 * operator==(T, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const T& lhs, const enumerator<T>& rhs)
{
	return lhs == rhs.value;
}

/**
 * operator==(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const bitmask<T>& lhs, const T& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(T, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const T& lhs, const bitmask<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const bitmask<T>& lhs, const enumerator<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(enumerator<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator==(const enumerator<T>& lhs, const bitmask<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const T& lhs, const T& rhs)
{
	return lhs != rhs;
}

/**
 * operator!=(enumerator<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const enumerator<T>& lhs, const enumerator<T>& rhs)
{
	return lhs.value != rhs.value;
}

/**
 * operator!=(bitmask<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const bitmask<T>& lhs, const bitmask<T>& rhs)
{
	return lhs.value != rhs.value;
}

/**
 * operator!=(enumerator<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const enumerator<T>& lhs, const T& rhs)
{
	return lhs.value != rhs;
}

/**
 * operator!=(T, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const T& lhs, const enumerator<T>& rhs)
{
	return lhs != rhs.value;
}

/**
 * operator!=(bitmask<T>, T)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const bitmask<T>& lhs, const T& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const T& lhs, const bitmask<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(bitmask<T>, enumerator<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const bitmask<T>& lhs, const enumerator<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(enumerator<T>, bitmask<T>)
 */
template <std::BitmaskTypeLike T>
constexpr auto operator!=(const enumerator<T>& lhs, const bitmask<T>& rhs)
{
	static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}


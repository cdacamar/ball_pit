module;
#include <type_traits>
export module Util.EnumUtils;

export
{

template <typename T>
concept Enum = std::is_enum_v<T>;

template <Enum E>
using PrimitiveType = std::underlying_type_t<E>;

template <Enum E>
constexpr auto rep(E e) { return PrimitiveType<E>(e); }

} // export
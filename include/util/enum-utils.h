#ifndef ENUM_UTILS_H
#define ENUM_UTILS_H

#include <type_traits>

template <typename T>
concept Enum = std::is_enum_v<T>;

template <Enum E>
using PrimitiveType = std::underlying_type_t<E>;

template <Enum E>
constexpr auto rep(E e) { return PrimitiveType<E>(e); }

#endif // ENUM_UTILS_H
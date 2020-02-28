/**
 * This file is part of the "dim" project
 *   Copyright (c) 2020 Christian Parpart <christian@parpart.family>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <linear_algebra>
#include "support.h"
#include <ostream>

#include <catch2/catch.hpp>

template <typename T>
class wrapped_number
{
  public:
    constexpr wrapped_number() = default;

    constexpr wrapped_number(T&& v) : value{std::forward<T>(v)} {}
    constexpr wrapped_number(T const& v) : value{v} {}

    constexpr wrapped_number(wrapped_number&&) = default;
    constexpr wrapped_number(wrapped_number const&) = default;

    constexpr wrapped_number& operator=(wrapped_number&&) = default;
    constexpr wrapped_number& operator=(wrapped_number const&) = default;

    constexpr wrapped_number operator+() const noexcept { return wrapped_number{+value}; }
    constexpr wrapped_number operator-() const noexcept { return wrapped_number{-value}; }

    constexpr wrapped_number operator+(wrapped_number const& rhs) const noexcept { return value + rhs.value; }
    constexpr wrapped_number operator-(wrapped_number const& rhs) const noexcept { return value - rhs.value; }
    constexpr wrapped_number operator*(wrapped_number const& rhs) const noexcept { return value * rhs.value; }
    constexpr wrapped_number operator/(wrapped_number const& rhs) const noexcept { return value / rhs.value; }

    constexpr wrapped_number& operator+=(wrapped_number const& rhs) const noexcept { value += rhs.value; return *this; }
    constexpr wrapped_number& operator-=(wrapped_number const& rhs) const noexcept { value -= rhs.value; return *this; }
    constexpr wrapped_number& operator*=(wrapped_number const& rhs) const noexcept { value *= rhs.value; return *this; }
    constexpr wrapped_number& operator/=(wrapped_number const& rhs) const noexcept { value /= rhs.value; return *this; }

    constexpr T get() const noexcept { return value; }

  private:
    T value{};
};

template <typename T> constexpr bool operator==(wrapped_number<T> const& lhs, wrapped_number<T> const& rhs) noexcept { return lhs.get() == rhs.get(); }
template <typename T> constexpr bool operator!=(wrapped_number<T> const& lhs, wrapped_number<T> const& rhs) noexcept { return lhs.get() != rhs.get(); }
template <typename T> constexpr bool operator==(T const& lhs, wrapped_number<T> const& rhs) noexcept { return lhs == rhs.get(); }
template <typename T> constexpr bool operator!=(T const& lhs, wrapped_number<T> const& rhs) noexcept { return lhs != rhs.get(); }
template <typename T> constexpr bool operator==(wrapped_number<T> const& lhs, T const& rhs) noexcept { return lhs.get() == rhs; }
template <typename T> constexpr bool operator!=(wrapped_number<T> const& lhs, T const& rhs) noexcept { return lhs.get() != rhs; }

template <typename T>
struct std::is_arithmetic<wrapped_number<T>> : public std::true_type {};

template <typename T>
std::ostream& operator<<(std::ostream& os, wrapped_number<T> const& num)
{
    return os << num.get() << "'";
}

template <typename T>
struct LINEAR_ALGEBRA_NAMESPACE::scalar_engine<wrapped_number<T>>
{
    using engine_category = scalar_engine_tag;
    using element_type = T;
};

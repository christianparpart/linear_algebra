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

#include "defs.h"

#include <algorithm>
#include <complex>
#include <cstddef>
#include <tuple>

namespace LINEAR_ALGEBRA_NAMESPACE::detail {

template <typename T> struct is_complex : public std::false_type {};
template <typename T> struct is_complex<std::complex<const T>> : public is_complex<T> {};
template <typename T> struct is_complex<std::complex<const volatile T>> : public is_complex<T> {};
template <typename T> struct is_complex<std::complex<volatile T>> : public is_complex<T> {};
template <typename T> struct is_complex<std::complex<T>> : public std::true_type {};
template <typename T> constexpr inline bool is_complex_v = is_complex<T>::value;

// ---------------------------------------------------------------------------------------------------

template <typename I, typename T>
struct _TimesIterator {
    T start;
    I count;
    T step;
    T current;

    constexpr T operator*() noexcept { return current; }
    constexpr T const& operator*() const noexcept { return current; }

    constexpr _TimesIterator<I, T>& operator++() noexcept { current += step; --count; return *this; }
    constexpr _TimesIterator<I, T>& operator++(int) noexcept { return ++*this; }

    constexpr _TimesIterator<I, T>& operator--() noexcept { current -= step; ++count; return *this; }
    constexpr _TimesIterator<I, T>& operator--(int) noexcept { return ++*this; }

    constexpr bool operator==(_TimesIterator<I, T> const& other) const noexcept { return count == other.count; }
    constexpr bool operator!=(_TimesIterator<I, T> const& other) const noexcept { return count != other.count; }
};

template <typename I, typename T>
struct _Times {
    T start;
    I count;
    T step;

    using iterator = _TimesIterator<I, T>;

    constexpr std::size_t size() const noexcept { return count; }
    constexpr T operator[](size_t i) const noexcept { return start + i * step; }

    constexpr iterator begin() const noexcept { return _TimesIterator<I, T>{start, count, step, start}; }

    constexpr iterator end() const noexcept {
        return iterator{
            start,
            I{},
            step,
            static_cast<T>(start + count * step)
        };
    }
};

template <typename I, typename T> _Times(T, I, T) -> _Times<I, T>;

// TODO: give random access hints to STL algorithms

template <typename I, typename T>
constexpr inline _Times<I, T> times(T start, I count, T step = 1)
{
    return _Times<I, T>{start, count, step};
}

template <typename T>
constexpr inline _Times<T, T> times(T count)
{
    return _Times<T, T>{0, count, 1};
}

// ---------------------------------------------------------------------------------------------------

template<
    typename I,
    typename T,
    typename Callable,
              typename std::enable_if_t<
                  std::is_invocable_r_v<void, Callable, T>,
                  int> = 0
>
constexpr void operator|(_Times<I, T> _times, Callable _callable)
{
    for (auto && i : _times)
        _callable(i);
}

// ---------------------------------------------------------------------------------------------------

template <typename I, typename T1, typename T2>
struct _Times2DIerator {
    using Outer = _Times<I, T1>;
    using Inner = _Times<I, T2>;

    Outer first;
    Inner second;
    typename Outer::iterator outerIt;
    typename Inner::iterator innerIt;

    constexpr _Times2DIerator(Outer _outer, Inner _inner, bool _init) noexcept :
        first{ std::move(_outer) },
        second{ std::move(_inner) },
        outerIt{ _init ? std::begin(first) : std::end(first) },
        innerIt{ _init ? std::begin(second) : std::end(second) }
    {}

    using value_type = std::tuple<T1, T2>;
    constexpr value_type operator*() const noexcept { return {*outerIt, *innerIt}; }

    constexpr _Times2DIerator<I, T1, T2>& operator++() noexcept {
        ++innerIt;
        if (innerIt == std::end(second)) {
            ++outerIt;
            if (outerIt != std::end(first))
                innerIt = std::begin(second);
        }
        return *this;
    }

    constexpr _Times2DIerator<I, T1, T2>& operator++(int) noexcept { return *++this; }

    constexpr bool operator==(_Times2DIerator<I, T1, T2> const& other) const noexcept {
        return innerIt == other.innerIt;
        //return outerIt == other.outerIt && innerIt == other.innerIt;
    }

    constexpr bool operator!=(_Times2DIerator<I, T1, T2> const& other) const noexcept {
        return !(*this == other);
    }
};

template <typename I, typename T1, typename T2>
struct _Times2D
{
    _Times<I, T1> first;
    _Times<I, T2> second;

    using iterator = _Times2DIerator<I, T1, T2>;

    constexpr std::size_t size() const noexcept { return first.size() * second.size(); }
    constexpr auto operator[](std::size_t i) const noexcept { return second[i % second.size()]; }

    constexpr iterator begin() const noexcept { return iterator{first, second, true}; }
    constexpr iterator end() const noexcept { return iterator{first, second, false}; }
};

template <typename I, typename T1, typename T2>
constexpr inline _Times2D<I, T1, T2> operator*(_Times<I, T1> a, _Times<I, T2> b)
{
    return _Times2D<I, T1, T2>{std::move(a), std::move(b)};
}

template<
    typename I,
    typename T1,
    typename T2,
    typename Callable,
              typename std::enable_if_t<
                  std::is_invocable_r_v<void, Callable, T1, T2>,
                  int> = 0
>
constexpr void operator|(_Times2D<I, T1, T2> _times, Callable _callable)
{
    for (auto && [i, j] : _times)
        _callable(i, j);
}

// ---------------------------------------------------------------------------------------------------
template <typename A, typename B>
constexpr auto zipped(A const& a, B const& b) noexcept
{
    struct Zipped {
        A const& a;
        B const& b;

        constexpr Zipped(A const& _a, B const& _b) noexcept : a{_a}, b{_b} {}

        struct iterator {
            decltype(std::begin(a)) ai;
            decltype(std::begin(b)) bi;

            constexpr auto operator*() const noexcept { return std::tuple{*ai, *bi}; }

            constexpr iterator& operator++() noexcept { ++ai; ++bi; return *this; }
            constexpr iterator& operator++(int) noexcept { return ++*this; }

            constexpr bool operator==(iterator const& x) const noexcept { return ai == x.ai && bi == x.bi; }
            constexpr bool operator!=(iterator const& x) const noexcept { return !(*this == x); }
        };

        constexpr auto begin() noexcept { return iterator{std::begin(a), std::begin(b)}; }
        constexpr auto end() noexcept { return iterator{std::end(a), std::end(b)}; }
    };
    return Zipped{a, b};
}

// ---------------------------------------------------------------------------------------------------

/**
 * Provides a facility for function composition.
 *
 * Suppose you write code like this:
 * <code>
 *   auto result = h(g(f(x)), x);
 * </code>
 *
 * What you can do, and what is much more readable:
 * <code>
 *   auto result = f(x) >> compose(g) >> compose(h, x);
 * </code>
 *
 * @param _fun the function to be composed with.
 * @param _args zero or more function parameters to be curried with the given function.
 *              The input resulting value of the left-hand-side of the function composition
 *              will be the last parameter being passed to _fun.
 *
 * @return the result of applying all parameters to the composed function.
 */
template <typename F, typename... Args>
constexpr auto compose(F _fun, Args... _args) -> std::pair<F, std::tuple<Args...>>
{
    return std::pair{std::move(_fun), std::make_tuple(std::forward<Args>(_args)...)};
}

/**
 * Function composition operator, to be used with the compose() function.
 */
template<typename S, typename F, typename... Args>
constexpr auto operator>>(S _input, std::pair<F, std::tuple<Args...>> _chain)
{
    return std::apply(
        _chain.first,
        std::tuple_cat(std::move(_chain.second), std::tuple<S>{std::move(_input)})
    );
}

// ---------------------------------------------------------------------------------------------------

template <typename Container, typename UnaryPred>
constexpr bool all_of(Container&& _container, UnaryPred _unaryPred)
{
    for (auto&& value : _container)
        if (!_unaryPred(value))
            return false;

    return true;
}

// constexpr/containered version of `std::for_each`
template <typename Container, typename Lambda>
constexpr void for_each(Container&& _container, Lambda&& lambda)
{
    for (auto&& value : _container)
        lambda(value);
}

// constexpr/containered version of `std::reduce(begin, end, init, op)``
template <typename Container, typename T, typename BinaryOp>
constexpr T reduce(Container&& _container, T&& _init, BinaryOp _binaryOp)
{
    auto result = T{std::move(_init)};
    for (auto&& value : _container)
        result = _binaryOp(result, value);
    return result;
}

// TODO: each version with ExecutionPolicy

} // end namespace

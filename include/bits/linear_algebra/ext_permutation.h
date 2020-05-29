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

#include "base.h"

#include <array>
#include <ostream>
#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace LINEAR_ALGEBRA_NAMESPACE {

namespace backport // {{{ constexpr versions of some algorithms (for pre C++20)
{
    template <class ForwardIt1, class ForwardIt2>
    constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
    {
       using std::swap;
       swap(*a, *b);
    }

    template <class BidirIt>
    constexpr void reverse(BidirIt first, BidirIt last)
    {
        while ((first != last) && (first != --last))
            iter_swap(first++, last);
    }

    template <class BidirIt>
    constexpr bool next_permutation(BidirIt first, BidirIt last)
    {
        if (first == last)
            return false;
        BidirIt i = last;
        if (first == --i)
            return false;
        while (true) {
            BidirIt i1, i2;
            i1 = i;
            if (*--i < *i1) {
                i2 = last;
                while (!(*i < *--i2))
                    ;
                iter_swap(i, i2);
                reverse(i1, last);
                return true;
            }
            if (i == first) {
                reverse(first, last);
                return false;
            }
        }
    }

    template<class ForwardIt>
    constexpr ForwardIt next(
        ForwardIt it,
        typename std::iterator_traits<ForwardIt>::difference_type n = 1)
    {
        while (n > 0)
        {
            ++it;
            --n;
        }
        return it;
    }
} // }}}

// compile-time factorial helper
template <std::size_t> struct factorial;
template <> struct factorial<1> { static constexpr std::size_t value = 1; };
template <std::size_t N> struct factorial { static constexpr std::size_t value = factorial<N - 1>::value * N; };
template <std::size_t N> constexpr inline std::size_t factorial_v = factorial<N>::value;

template <std::size_t N>
class permutation;

// {{{ free functions
template <std::size_t N>
auto to_map(permutation<N> const& pi) -> std::map<typename permutation<N>::value_type,
                                                  typename permutation<N>::value_type>
{
    using value_type = typename permutation<N>::value_type;
    using map_type = std::map<value_type, value_type>;
    map_type m;
    for (std::size_t i : detail::times(1, pi.size()))
        m[i] = pi(i);
    return m;
}

template <std::size_t N>
std::string raw_form(permutation<N> const& pi)
{
    return detail::wrapped(
        std::stringstream{},
        [](auto& s) { s << '('; },
        [&](auto& s) {
            detail::joined(
                detail::times(1, pi.size()),
                [&]() { s << ' '; },
                [&](auto n) { s << pi(n); }
            );
        },
        [](auto& s) { s << ')'; }
    ).str();
}

template <std::size_t N>
std::string simple_form(permutation<N> const& pi)
{
    return detail::joined(
        detail::times(1, pi.size()),
        std::stringstream{},
        [](auto& s) { s << ' '; },
        [&](auto& s, auto n) {s << '(' << n << ' ' << pi(n) << ')';}
    ).str();
}

template<std::size_t N>
std::string canonical_form(permutation<N> const& pi)
{
    using value_type = typename permutation<N>::value_type;
    using map_type = std::map<value_type, value_type>;
    using cycles_type = std::list<value_type>;

    // Finds largest key in map.
    auto const largest_key = [](map_type const& m) -> value_type {
        value_type largest = 0;
        for (auto const& item : m)
            if (item.first > largest)
                largest = item.first;
        return largest;
    };

    // Builds a single cycle with given start value from pending map and removes the cycle from pending.
    auto const build_cycle = [](value_type start, map_type& pending) -> cycles_type {
        auto acc = cycles_type{start};
        for (;;)
        {
            auto const next = pending[acc.back()];
            pending.erase(acc.back());
            if (next == start)
                break;
            acc.push_back(next);
        }
        return acc;
    };

    auto pending = to_map(pi);
    auto cycles = std::list<cycles_type>{};
    while (!pending.empty())
        cycles.emplace_back(build_cycle(largest_key(pending), pending));

    return detail::joined(
        cycles,
        std::stringstream{},
        [](auto& s) { s << ' '; },
        [](auto& s, auto const& cycle) {
            s << '(';
            detail::joined(
                cycle,
                [&]() { s << ' '; },
                [&](auto ci) { s << ci; }
            );
            s << ')';
        }
    ).str();
}

template<std::size_t N>
std::string to_string(permutation<N> const& pi)
{
    return canonical_form(pi);
}
// }}}

// TODO: split up into permutation<ET> and fs_permutation_engine<N> and dr_permutation_engine

template <std::size_t N>
class permutation {
  public:
    using value_type = unsigned;
    using size_type = value_type;
    using map_type = std::map<value_type, value_type>;
    using array_type = std::array<value_type, N + 1>;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;
    using difference_type = ptrdiff_t;
    using iterator = typename array_type::iterator;
    using const_iterator = typename array_type::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    template<
        typename Initializer,
        typename std::enable_if_t<
            std::is_invocable_r_v<value_type, Initializer, value_type>,
            int> = 0
    >
    constexpr explicit permutation(Initializer const& _init) noexcept
    {
        for (auto const from : detail::times(1, N))
            values_[from] = _init(static_cast<value_type>(from));
    }

    // Creates a permutation of a list of mappings. The list's head position marks the first input value and its value (1) the permutation.
    constexpr permutation(std::initializer_list<value_type> values)
    {
        std::size_t i = 1;
        for (auto const v : values)
            values_[i++] = v;
    }

    // Creates a Permutation represented as a list of unordered transpositions.
    constexpr permutation(std::initializer_list<std::pair<value_type, value_type>> values)
        : permutation()
    {
        for (auto const [i, v] : values)
            values_[i] = v;
    }

    constexpr permutation()
        : permutation([](value_type i) { return i; })
    {
    }

    constexpr permutation(const permutation&) noexcept = default;
    constexpr permutation& operator=(const permutation&) noexcept = default;
    constexpr permutation(permutation&&) noexcept = default;
    constexpr permutation& operator=(permutation&&) noexcept = default;

    constexpr size_type size() const noexcept { return N; }

    constexpr value_type operator()(value_type n) const noexcept
    {
        if (n >= 1 && n <= size())
            return values_[n];
        else
            return n;
    }

    constexpr value_type operator[](value_type n) const noexcept
    {
        return (*this)(n);
    }

    constexpr value_type inverse(value_type n) const noexcept
    {
        for (size_t i = 0; i < values_.size(); ++i)
            if (values_[i] == n)
                return static_cast<value_type>(i);
        return n;
    }

    constexpr iterator begin() { return backport::next(values_.begin()); }
    constexpr iterator end() { return values_.end(); }
    constexpr const_iterator cbegin() { return backport::next(values_.cbegin()); }
    constexpr const_iterator cend() { return values_.cend(); }

    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

    constexpr static permutation<N> identity()
    {
        return permutation<N>();
    }

    // Constructs a list of all permutations between 1 and n.
    constexpr static std::array<permutation<N>, factorial_v<N>> all()
    {
        std::array<permutation<N>, factorial_v<N>> r{};
        for (size_t i = 1; i < factorial_v<N>; ++i)
            ++(r[i] = r[i - 1]);
        return r;
    }

    constexpr permutation& operator++() noexcept
    {
        backport::next_permutation(begin(), end());
        return *this;
    }

  private:
    array_type values_{};
};

template <std::size_t N>
constexpr bool operator==(permutation<N> const& a, permutation<N> const& b) noexcept
{
    if (a.size() == b.size())
        for (std::size_t i = 0; i < a.size(); ++i)
            if (!(a(i) == b(i)))
                return false;

    return true;
}

template <std::size_t N>
constexpr bool operator!=(permutation<N> const& a, permutation<N> const& b) noexcept
{
    return !(a == b);
}

template <std::size_t N>
inline std::ostream& operator<<(std::ostream& os, permutation<N> const& pi)
{
    return os << to_string(pi);
}

// Permutation composition.
// a * b: x -> a(b(x))
template <std::size_t N>
constexpr permutation<N> operator*(permutation<N> const& a, permutation<N> const& b) noexcept
{
    return permutation<N>([&](auto v) { return a(b(v)); });
}

// Creates an inverse permutation of the input permutation @p p.
template <std::size_t N>
constexpr permutation<N> inverse(permutation<N> const& p) noexcept
{
    return permutation<N>([&](auto n) { return p.inverse(n); });
}

// Creates a Permutation represented as a list of unordered transpositions.
// TODO: ofTranspositions (size: int) (_values: Transposition list) =

template <std::size_t N>
constexpr std::size_t failure_count(permutation<N> const& pi) noexcept
{
#if 1
    std::size_t c = 0;
    for (std::size_t n = 0; n <= N; ++n)
        for (std::size_t i = 0; i <= n; ++i)
            if (pi(i) > pi(n))
                c++;
    return c;
#else
    return detail::reduce(
        detail::times(pi.size()),
        0,
        [&pi](auto a, auto i) {
            return std::reduce(
                detail::times(i - 1),
                a,
                [&pi, i](auto a, auto j) {
                    return pi(i) > pi(j) ? 1 : 0;
                }
            );
        }
    );
#endif
}

template <std::size_t N>
constexpr std::size_t is_even(permutation<N> const& pi) noexcept
{
    return failure_count(pi) % 2 == 0;
}

template <std::size_t N>
constexpr std::size_t is_odd(permutation<N> const& pi) noexcept
{
    return !is_even(pi);
}

template <std::size_t N>
constexpr int sgn(permutation<N> const& pi) noexcept
{
    if (is_even(pi))
        return +1;
    else
        return -1;
}

} // end namespace

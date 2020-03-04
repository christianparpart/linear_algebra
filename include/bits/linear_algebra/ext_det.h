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
#include "matrix.h"
#include "fs_matrix_engine.h"
#include "dr_matrix_engine.h"
#include "ext_permutation.h"
#include "submatrix_engine.h"
#include "support.h"
#include "concepts.h"

#include <cassert>

namespace LINEAR_ALGEBRA_NAMESPACE {

namespace detail { // {{{
    template <typename ET, typename OT>
    constexpr auto weigh_trivials_in_row(matrix<ET, OT> const& m, size_t i)
    {
        typename ET::size_type c = 0;
        for (auto j : times(0u, m.columns()))
        {
            auto const v = m(i, j);
            auto const w = v < 0 ? -v : v;
            if (w == 0 || w == 1 || w == 2)
                c += 3 - w;
        }
        return c;
    }

    template <typename ET, typename OT>
    constexpr auto weigh_trivials_in_column(matrix<ET, OT> const& m, size_t j)
    {
        std::size_t c = 0;
        for (std::size_t i : times(0u, m.rows()))
        {
            auto const v = m(i, j);
            auto const w = v < 0 ? -v : v;
            if (w == 0 || w == 1 || w == 2)
                c += 3 - w;
        }

        return c;
    }

    /**
     * Finds the best matrix row or column to expand by.
     *
     * @returns a pair whose first argument is the row or column index and the second argument
     *          determines whether or not the first one is a row index (column index otherwise).
     */
    template <typename ET, typename OT>
    constexpr std::pair<std::size_t, bool> find_expansion_vector(matrix<ET, OT> const& a) noexcept
    {
        std::size_t expansion_index = 0;
        std::size_t most_trivials = weigh_trivials_in_row(a, 0);

        for (std::size_t k : times(1u, a.columns() - 1))
            if (auto const c = weigh_trivials_in_row(a, k); c > most_trivials)
            {
                expansion_index = k;
                most_trivials = c;
            }

        bool expandByRow = true;
        for (std::size_t k : times(0u, a.columns()))
            if (auto const c = weigh_trivials_in_column(a, k); c > most_trivials)
            {
                expandByRow = false;
                expansion_index = k;
                most_trivials = c;
            }

        return {expansion_index, expandByRow};
    }
} // }}}

#if 0 // {{{ det(A)
template <typename ET, typename OT>
constexpr auto det(matrix<ET, OT> const& m) // -> typename matrix<ET, OT>::value_type
{
    using detail::times;
    using detail::reduce;

    if (m.rows() != m.columns())
        return 0; // TODO
    else if (m.rows() == 3)
        return m(0, 0) * m(1, 1) * m(2, 2)
             + m(1, 0) * m(2, 1) * m(0, 2)
             + m(2, 0) * m(0, 1) * m(1, 2)
             - m(2, 0) * m(1, 1) * m(0, 2)
             - m(2, 1) * m(1, 2) * m(0, 0)
             - m(2, 2) * m(1, 0) * m(0, 1);
    else if (m.rows() == 2)
        return m(0, 0) * m(1, 1)
             - m(1, 0) * m(0, 1);
    else if (m.rows() == 1)
        return m(0, 0);
    else
    {
        using detail::times;
        using detail::reduce;

        auto const [expansion_index, expandByRow] = detail::find_expansion_vector(m);

        if (expandByRow)
        {
            std::size_t const i = expansion_index;
            return reduce(
                times(m.columns()),
                0,
                [&](auto acc, auto j) constexpr {
                    return (i + j) % 2 == 0
                        ? acc + m(i, j) * det(m.submatrix(i, j))
                        : acc - m(i, j) * det(m.submatrix(i, j));
                }
            );
        }
        else // expand by row
        {
            std::size_t const j = expansion_index;
            return reduce(
                times(m.columns()),
                0,
                [&](auto acc, auto i) constexpr {
                    return (i + j) % 2 == 0
                        ? acc + m(i, j) * det(m.submatrix(i, j))
                        : acc - m(i, j) * det(m.submatrix(i, j));
                }
            );
        }
    }
}

template <class T, std::size_t R, std::size_t C>
constexpr T det(matrix<fs_matrix_engine<T, R, C>> const& a) LA_CONCEPT(R == C)
{
    using detail::times;
    using detail::reduce;

    auto const [expansion_index, expandByRow] = detail::find_expansion_vector(a);

    if (expandByRow)
    {
        std::size_t const i = expansion_index;
        return reduce(
            times(C),
            0,
            [&](auto acc, auto j) constexpr {
                return (i + j) % 2 == 0
                    ? acc + a(i, j) * det(a.submatrix(i, j))
                    : acc - a(i, j) * det(a.submatrix(i, j));
            }
        );
    }
    else // expand by row
    {
        std::size_t const j = expansion_index;
        return reduce(
            times(a.columns()),
            0,
            [&](auto acc, auto i) constexpr {
                return (i + j) % 2 == 0
                    ? acc + a(i, j) * det(a.submatrix(i, j))
                    : acc - a(i, j) * det(a.submatrix(i, j));
            }
        );
    }
}
#endif // }}}

template <class T, class OT> constexpr T det(matrix<fs_matrix_engine<T, 1, 1>, OT> const& m)
{
    return m(0, 0);
}

template <class T, class OT> constexpr T det(matrix<fs_matrix_engine<T, 2, 2>, OT> const& m)
{
    return m(0, 0) * m(1, 1)
         - m(1, 0) * m(0, 1);
}

template <class T, class OT> constexpr T det(matrix<fs_matrix_engine<T, 3, 3>, OT> const& m)
{
    return m(0, 0) * m(1, 1) * m(2, 2)
         + m(1, 0) * m(2, 1) * m(0, 2)
         + m(2, 0) * m(0, 1) * m(1, 2)
         - m(2, 0) * m(1, 1) * m(0, 2)
         - m(2, 1) * m(1, 2) * m(0, 0)
         - m(2, 2) * m(1, 0) * m(0, 1);
}

template <class T, class OT, std::size_t N> constexpr T det(matrix<fs_matrix_engine<T, N, N>, OT> const& m)
{
    return detail::reduce(
        permutation<N>::all(),
        T{},
        [&](T acc, auto const& pi) {
            return acc + sgn(pi) * detail::reduce(
                detail::times(1, pi.size()),
                T{1},
                [&](auto a, auto i) constexpr { return a * m(i - 1, pi(i) - 1); });
        }
    );
}

template <class T, class OT, std::size_t N> constexpr T det(
    matrix<submatrix_engine<fs_matrix_engine<T, N, N>,
                            typename fs_matrix_engine<T, N, N>::engine_category>, OT> const& m)
{
    return detail::reduce(
        permutation<N>::all(),
        T{},
        [&](T acc, auto const& pi) {
            return acc + sgn(pi) * detail::reduce(
                detail::times(1, pi.size()),
                T{1},
                [&](auto a, auto i) constexpr { return a * m(i - 1, pi(i) - 1); });
        }
    );
}

#if 1 // {{{ det(A) Leibnitz

#endif // }}}

/// Tests whether or not given matrix is invertible.
template <typename ET, typename OT>
constexpr bool is_invertible(matrix<ET, OT> const& m)
{
    auto const zero = typename ET::value_type{};
    return det(m) != zero;
}

/// Computes the cofactor of a square matrix m at given position (i, j).
template <typename ET, typename OT, typename size_type = typename ET::size_type>
constexpr auto cofactor(matrix<ET, OT> const& m, size_type i, size_type j) -> typename ET::value_type
{
    return (i + j) % 2 == 0 ? +det(m.submatrix(i, j))
                            : -det(m.submatrix(i, j));
}

/// Compute the cofactor matrix of a square matrix.
template <typename ET, typename OT>
constexpr matrix<ET, OT> cofactor(matrix<ET, OT> const& m)
{
    return matrix<ET, OT>(m.rows(), m.columns(), [&](auto i, auto j) { return cofactor(m, i, j); });
}

/// Compute the cofactor matrix of a square matrix.
template <typename ET, typename OT>
constexpr matrix<ET, OT> adjugate(matrix<ET, OT> const& m)
{
    //return cofactor(m).t();
    if constexpr (is_resizable_engine_v<ET>)
    {
        assert(m.rows() == m.columns());
        return matrix<ET, OT>(m.columns(), m.rows(), [&](auto i, auto j) { return cofactor(m, j, i); });
    }
    else
    {
        // TODO: find a way so that OS/X's Clang is not complaining in the static_assert below.
        // static_assert(m.rows() == m.columns());
        return matrix<ET, OT>([&](auto i, auto j) { return cofactor(m, j, i); });
    }
}

/// Computes the inverse matrix of a square matrix.
template <typename ET, typename OT>
constexpr auto inverse(matrix<ET, OT> const& m)
{
    // TODO
    // if constexpr (is_fixed_size_engine_v<ET>)
    //     static_assert(is_invertible(m));

    auto const detM = det(m);
    // auto const zero = typename ET::value_type{};
    // if (detM == zero)
    //     throw std::domain_error{"Given matrix is not invertible."};

    auto const one = typename ET::value_type(1);
    return one / detM * adjugate(m);
}

} // end namespace

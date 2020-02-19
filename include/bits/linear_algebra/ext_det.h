#pragma once

#include "base.h"
#include "matrix.h"
#include "fs_matrix_engine.h"
#include "dr_matrix_engine.h"
#include "support.h"
#include "concepts.h"

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

template <typename T, typename AT, typename OT>
constexpr auto det(matrix<dr_matrix_engine<T, AT>, OT> const& m) // -> typename matrix<ET, OT>::value_type
{
    using detail::times;
    using detail::reduce;
    using value_type = typename matrix<dr_matrix_engine<T, AT>, OT>::value_type;

    if (m.rows() != m.columns())
        return value_type{};
    else
        // return value_type{};
        return reduce(
            times(m.columns()),
            0,
            [&](auto acc, auto j) constexpr {
                std::size_t const i = 0;
                return (i + j) % 2 == 0
                    ? acc + m(i, j) * det(materialize(m.submatrix(i, j)))
                    : acc - m(i, j) * det(materialize(m.submatrix(i, j)));
            }
        );
}

template <class OT, class T, std::size_t R, std::size_t C>
constexpr T det(matrix<fs_matrix_engine<T, R, C>, OT> const& a) LA_CONCEPT(R == C)
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
                    ? acc + a(i, j) * det(materialize(a.submatrix(i, j)))
                    : acc - a(i, j) * det(materialize(a.submatrix(i, j)));
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
                auto const m = materialize(a.submatrix(i, j));
                auto const detM = det(m);
                return (i + j) % 2 == 0
                    ? acc + a(i, j) * detM
                    : acc - a(i, j) * detM;
                // return (i + j) % 2 == 0
                //     ? acc + a(i, j) * det(materialize(a.submatrix(i, j)))
                //     : acc - a(i, j) * det(materialize(a.submatrix(i, j)));
            }
        );
    }
}

template <class OT, class T> constexpr T det(matrix<fs_matrix_engine<T, 1, 1>, OT> const& m)
{
    return m(0, 0);
}

template <class OT, class T> constexpr T det(matrix<fs_matrix_engine<T, 2, 2>, OT> const& m)
{
    return m(0, 0) * m(1, 1)
         - m(1, 0) * m(0, 1);
}

template <class OT, class T> constexpr T det(matrix<fs_matrix_engine<T, 3, 3>, OT> const& m)
{
    return m(0, 0) * m(1, 1) * m(2, 2)
         + m(1, 0) * m(2, 1) * m(0, 2)
         + m(2, 0) * m(0, 1) * m(1, 2)
         - m(2, 0) * m(1, 1) * m(0, 2)
         - m(2, 1) * m(1, 2) * m(0, 0)
         - m(2, 2) * m(1, 0) * m(0, 1);
}

} // end namespace

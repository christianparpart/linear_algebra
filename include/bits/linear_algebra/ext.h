#pragma once

#include "base.h"
#include "vector.h"
#include "matrix.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) noexcept
{
    // TODO: replace naive implementation, use of std::equal?
    if (v1.size() == v2.size())
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            if (!(v1(i) == v2(i)))
                return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    // TODO: replace naive implementationbb:w, use of std::equal?
    if (m1.rows() == m2.rows() && m1.columns() == m2.columns())
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                if (!(m1(i, j) == m2(i, j)))
                    return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator!=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    return !(m1 == m2);
}

} // end namespace

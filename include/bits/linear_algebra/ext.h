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
#include "vector.h"
#include "matrix.h"

#include <ostream>

namespace LINEAR_ALGEBRA_NAMESPACE {

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) noexcept
{
    // TODO: replace naive implementation, use of std::equal?
    if (v1.size() != v2.size())
        return false;

    using detail::times;
    for (auto i : times(v1.size()))
        if (!(v1(i) == v2(i)))
            return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    // TODO: replace naive implementationbb:w, use of std::equal?
    if (m1.rows() != m2.rows() || m1.columns() != m2.columns())
        return false;

    using detail::times;
    for (auto [i, j] : times(m1.rows()) * times(m1.columns()))
        if (!(m1(i, j) == m2(i, j)))
            return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator!=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    return !(m1 == m2);
}

template <typename ET, typename OT>
std::ostream& operator<<(std::ostream& os, vector<ET, OT>const& _vec)
{
    os << '(';
    for (std::size_t i = 0; i < _vec.size(); ++i)
    {
        if (i != 0)
            os << ", ";
        os << _vec(i);
    }
    os << ')';
    return os;
}

template <typename ET, typename OT>
std::ostream& operator<<(std::ostream& os, matrix<ET, OT> const& _mat)
{
    os << '{';
    for (std::size_t i = 0; i < _mat.rows(); ++i)
    {
        if (i != 0)
            os << ", ";
        os << '{';
        for (std::size_t j = 0; j < _mat.columns(); ++j)
        {
            if (j != 0)
                os << ", ";
            os << _mat(i, j);
        }
        os << '}';
    }
    os << '}';
    return os;
}

} // end namespace

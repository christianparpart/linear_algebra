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
#include "support.h"
#include "vector.h"
#include "matrix.h"
#include "convenience_aliases.h"

#include <ostream>

namespace LINEAR_ALGEBRA_NAMESPACE {

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) noexcept
{
    using detail::times;
    return v1.size() == v2.size() && all_of(times(v1.size()), [&](auto i) { return v1(i) == v2(i); });
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator!=(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) noexcept
{
    return !(v1 == v2);
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    using detail::times;
    using detail::all_of;
    return m1.size() == m2.size()
        && all_of(times(m1.rows()) * times(m2.columns()), [&](auto ij) {
            return m1(std::get<0>(ij), std::get<1>(ij))
                == m2(std::get<0>(ij), std::get<1>(ij));
        });
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator!=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    return !(m1 == m2);
}

template <typename ET, typename OT>
std::ostream& operator<<(std::ostream& os, vector<ET, OT>const& _vec)
{
    using detail::times;
    os << '(';
    for (auto i : times(_vec.size()))
        os << (i != 0 ? ", " : "") << _vec(i);
    os << ')';
    return os;
}

template <typename ET, typename OT>
std::ostream& operator<<(std::ostream& os, matrix<ET, OT> const& _mat)
{
    using detail::times;
    os << '{';
    for (auto [i, j] : times(_mat.rows()) * times(_mat.columns()))
        os << (j == 0 && i != 0 ? "}, " : "")   // row end
           << (j == 0 ? "{" : ", ")             // row begin
           << _mat(i, j);
    os << "}}";
    return os;
}

} // end namespace

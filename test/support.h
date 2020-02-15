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
#include <ostream>

namespace { // {{{ helper
    template <typename ET, typename OT>
    std::ostream& operator<<(std::ostream& os, LINEAR_ALGEBRA_NAMESPACE::vector<ET, OT>const& _vec)
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
    std::ostream& operator<<(std::ostream& os, LINEAR_ALGEBRA_NAMESPACE::matrix<ET, OT> const& _mat)
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
} // }}}

template <typename T, size_t N> using vec = LINEAR_ALGEBRA_NAMESPACE::fs_vector<T, N>;
template <size_t N> using ivec = vec<int, N>;

template <typename T, size_t R, size_t C> using mat = LINEAR_ALGEBRA_NAMESPACE::fs_matrix<T, R, C>;
template <size_t R, size_t C> using imat = mat<int, R, C>;


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

#include <ostream>
#include <linear_algebra>

#include <catch2/catch.hpp>

// XXX later on, split up in per-operation tests (negation, addition, subtraction, multiplication, ...)

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

using namespace std;

TEST_CASE("vector.neg")
{
    auto static constexpr v1 = ivec<3>{0, 1, 2};
    auto static const v2 = -v1;
    auto static const v3 = ivec<3>{0, -1, -2};
    REQUIRE(v2 == v3);
}

TEST_CASE("vector.add")
{
    auto static constexpr v1 = ivec<3>{0, 1, 2};
    auto static constexpr v2 = ivec<3>{3, 4, 5};
    auto static constexpr v3 = v1 + v2;
    auto static constexpr expected = ivec<3>{3,5,7};
    REQUIRE(v3 == expected);
}

TEST_CASE("vector.sub")
{
    auto static constexpr v1 = ivec<3>{5, 2, 1};
    auto static constexpr v2 = ivec<3>{1, 2, 5};
    auto static constexpr v3 = v1 - v2;
    auto static constexpr expected = ivec<3>{4, 0, -4};
    REQUIRE(v3 == expected);
}

TEST_CASE("vector.mul")
{
    SECTION("inner")
    {
        auto static constexpr v1 = ivec<3>{5, 2, 3};
        auto static constexpr v2 = ivec<3>{1, 2, 5};
        auto static constexpr v3 = v1 * v2;
        auto static constexpr expected = 24;
        REQUIRE(v3 == expected);
    }

    // SECTION("scalar left")
    // {
    //     auto static constexpr v1 = ivec<3>{5, 2, 3};
    //     auto static constexpr s1 = 4;
    //     auto static constexpr v2 = s1 * v1;
    //     auto static constexpr expected = ivec<3>{20, 8, 12};
    //     REQUIRE(v2 == expected);
    // }

    // SECTION("scalar right")
    // {
    //     auto static constexpr v1 = ivec<3>{5, 2, 3};
    //     auto static constexpr s1 = 4;
    //     auto static constexpr v2 = v1 * s1;
    //     auto static constexpr expected = ivec<3>{20, 8, 12};
    //     REQUIRE(v2 == expected);
    // }
}

TEST_CASE("matrix.neg")
{
    auto static constexpr m1 = imat<2, 2>{1,-2,3,-4};
    auto static constexpr m2 = -m1;
    auto static constexpr m3 = imat<2, 2>{-1,2,-3,4};
    REQUIRE(m2 == m3);
}

TEST_CASE("matrix.add")
{
    auto static constexpr m1 = imat<2, 2>{1,2,3,4};
    auto static constexpr m2 = imat<2, 2>{2,3,4,5};
    auto static constexpr m3 = m1 + m2;
    auto static constexpr expected = imat<2, 2>{3,5,7,9};
    REQUIRE(m3 == expected);
}

TEST_CASE("matrix.sub")
{
    auto static constexpr m1 = imat<2, 2>{5,2,1,0};
    auto static constexpr m2 = imat<2, 2>{0,2,4,8};
    auto static constexpr m3 = m1 - m2;
    auto static constexpr expected = imat<2, 2>{5,0,-3,-8};
    REQUIRE(m3 == expected);
}

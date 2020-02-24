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

#include <linear_algebra>
#include "support.h"
#include "wrapped_number.h"
#include <ostream>

#include <catch2/catch.hpp>

namespace la = LINEAR_ALGEBRA_NAMESPACE;

using inum = wrapped_number<int>;

template <std::size_t R, std::size_t C>
using cmat = la::matrix<la::fs_matrix_engine<inum, R, C>, la::matrix_operation_traits>;

TEST_CASE("custom_number.matrix")
{
    auto const m1 = cmat<2, 3>{3, 4, 5,
                               7, 8, 9};
    auto const m2 = imat<2, 3>{1, 2, 3,
                               4, 5, 6};

    SECTION("construction") {
        CHECK(m1 == cmat<2, 3>{3, 4, 5, 7, 8, 9});
        CHECK(m1 == imat<2, 3>{3, 4, 5, 7, 8, 9});
    }

    SECTION("neg") {
        auto const negated = -m1;
        auto const negatedE = cmat<2, 3>{-3, -4, -5,
                                         -7, -8, -9};
        static_assert(std::is_same_v<decltype(negated)::value_type, inum>);
        static_assert(std::is_same_v<std::remove_cv_t<decltype(negated)>, cmat<2, 3>>);
        CHECK(negated == negatedE);
    }

    SECTION("add") {
        auto const sum = m1 + m2;
        auto const sumE = cmat<2, 3>{4,  6,   8,
                                     11, 13, 15};
        static_assert(std::is_same_v<decltype(sum)::value_type, inum>);
        static_assert(std::is_same_v<std::remove_cv_t<decltype(sum)>, cmat<2, 3>>);
        CHECK(sum == sumE);
    }

    SECTION("sub") {
        auto const zero = m1 - m1;
        static_assert(std::is_same_v<decltype(zero)::value_type, inum>);
        static_assert(std::is_same_v<std::remove_cv_t<decltype(zero)>, cmat<2, 3>>);
        CHECK(zero == cmat<2, 3>{});
    }

    SECTION("mul") {
        auto const m3 = cmat<3, 1>{2, 1, 3};
        auto const m4 = m1 * m3;
        auto const me = cmat<2, 1>{25, 49};
        CHECK(me == m4);
        CHECK(m4 == me);
    }
}

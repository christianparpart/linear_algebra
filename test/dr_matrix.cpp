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
#include "support.h"

#include <catch2/catch.hpp>

TEST_CASE("dr_matrix.ctor")
{
    auto const me = dmat<int>(imat<3, 4>{0, 1, 2, 3,
                                         4, 5, 6, 7,
                                         8, 9, 10, 11});

    SECTION("lambda initializer")
    {
        auto const m1 = dmat<int>(imat<3, 4>([](auto i, auto j) constexpr { return i * 4 + j; }));
        REQUIRE(m1 == me);
    }

    SECTION("copy")
    {
        auto const m2 = me;
        REQUIRE(m2 == me);
    }
}


TEST_CASE("dr_matrix.row")
{
    auto const m1 = dmat<int>(imat<3, 4>{1, 2, 3, 4,
                                         2, 3, 4, 5,
                                         3, 4, 5, 6});

    SECTION("read")
    {
        CHECK(m1.row(0) == ivec<4>{1, 2, 3, 4});
        CHECK(m1.row(1) == ivec<4>{2, 3, 4, 5});
        CHECK(m1.row(2) == ivec<4>{3, 4, 5, 6});
    }

    SECTION("write")
    {
        auto const me = dmat<int>(imat<3, 4>{1, 2, 3, 4,
                                             2,11, 4,13,
                                             3, 4, 5, 6});
        dmat<int> m2 = m1;
        dmat<int>::row_type r = m2.row(1);
        // r(1) = 11;
        // r(3) = 13;
        // m2.row(1)(1) = 11;
        // m2.row(1)(3) = 13;
        // CHECK(m2.row(1) == ivec<4>{2, 11, 4, 13});
        // CHECK(m2 == me);
    }
}


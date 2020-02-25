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

TEST_CASE("dr_matrix.resize")
{
    auto const m1 = dmat<int>(imat<3, 4>{1, 2, 3, 4,
                                         2, 3, 4, 5,
                                         3, 4, 5, 6});

    SECTION("smaller 1")
    {
        auto m2 = m1;
        m2.resize(2, 3);
        CHECK(m2 == imat<2, 3>{1, 2, 3, 2, 3, 4});
    }

    SECTION("smaller 2")
    {
        auto m2 = m1;
        m2.resize(2, 1);
        CHECK(m2 == imat<2, 1>{1, 2});
    }

    SECTION("smaller 3")
    {
        auto m2 = m1;
        m2.resize(1, 1);
        CHECK(m2 == imat<1, 1>{1});
    }

    SECTION("bigger 1") {
        auto m2 = m1;
        m2.resize(4, 5);
        auto const me = dmat<int>(imat<4, 5>{1, 2, 3, 4, 0,
                                             2, 3, 4, 5, 0,
                                             3, 4, 5, 6, 0,
                                             0, 0, 0, 0, 0});
        CHECK(m2 == me);
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
        dmat<int> const m2 = m1;
        auto r = m2.row(1);
        r(1) = 11;
        r(3) = 13;
        CHECK(m2.row(1) == ivec<4>{2, 11, 4, 13});
        CHECK(m2 == me);
    }
}

TEST_CASE("dr_matrix.column")
{
    auto static const m1 = dmat<int>(imat<3, 4>{1, 2, 3, 4,
                                                2, 3, 4, 5,
                                                3, 4, 5, 6});

    SECTION("read")
    {
        CHECK(m1.column(0) == ivec<3>{1, 2, 3});
        CHECK(m1.column(1) == ivec<3>{2, 3, 4});
        CHECK(m1.column(2) == ivec<3>{3, 4, 5});
        CHECK(m1.column(3) == ivec<3>{4, 5, 6});
    }

    SECTION("write")
    {
        auto const me = imat<3, 4>{1, 2, 3, 7,
                                   2, 3, 4, 6,
                                   3, 4, 5, 5};
        auto m2 = m1;
        auto c23 = m2.column(3);
        c23(0) = 7;
        c23(1) = 6;
        c23(2) = 5;
        CHECK(m2.column(3) == ivec<3>{7, 6, 5});
        CHECK(m2 == me);
    }
}

TEST_CASE("dr_matrix.submatrix")
{
    auto const base = dmat<int>(imat<4, 5>{ 0,  1,  2,  3,  4,
                                            10, 11, 12, 13, 14,
                                            20, 21, 22, 23, 24,
                                            30, 31, 32, 33, 34});
    SECTION("single") {
        auto const m1 = base.submatrix(2, 3);
        auto const me = dmat<int>(imat<3, 4>{ 0,  1,  2,  4,
                                              10, 11, 12, 14,
                                              30, 31, 32, 34});
        REQUIRE(m1 == me);
    }

    SECTION("multiple") {
        auto const m1 = base.submatrix(1, 2, 1, 3);
        auto const me = imat<2, 2>{ 0,  4,
                                    30, 34};
        REQUIRE(m1 == me);
    }
}


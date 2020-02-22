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

TEST_CASE("matrix.ctor")
{
    auto static constexpr me = imat<3, 4>{0, 1, 2, 3,
                                          4, 5, 6, 7,
                                          8, 9, 10, 11};

    SECTION("lambda initializer")
    {
        auto static constexpr m1 = imat<3, 4>([](auto i, auto j) constexpr { return i * 4 + j; });
        REQUIRE(m1 == me);
    }

    SECTION("copy")
    {
        auto static constexpr copy = me;
        REQUIRE(copy == me);
    }
}

TEST_CASE("matrix.row")
{
    auto static constexpr m1 = imat<3, 4>{1, 2, 3, 4,
                                          2, 3, 4, 5,
                                          3, 4, 5, 6};

    SECTION("read")
    {
        CHECK(m1.row(0) == ivec<4>{1, 2, 3, 4});
        CHECK(m1.row(1) == ivec<4>{2, 3, 4, 5});
        CHECK(m1.row(2) == ivec<4>{3, 4, 5, 6});
    }

    SECTION("write")
    {
        auto static constexpr me = imat<3, 4>{1, 2, 3, 4,
                                              2,11, 4,13,
                                              3, 4, 5, 6};
        auto m2 = m1;
        m2.row(1)(1) = 11;
        m2.row(1)(3) = 13;
        CHECK(m2.row(1) == ivec<4>{2, 11, 4, 13});
        CHECK(m2 == me);
    }
}

TEST_CASE("matrix.column")
{
    auto static constexpr m1 = imat<3, 4>{1, 2, 3, 4,
                                          2, 3, 4, 5,
                                          3, 4, 5, 6};

    SECTION("read")
    {
        CHECK(m1.column(0) == ivec<3>{1, 2, 3});
        CHECK(m1.column(1) == ivec<3>{2, 3, 4});
        CHECK(m1.column(2) == ivec<3>{3, 4, 5});
        CHECK(m1.column(3) == ivec<3>{4, 5, 6});
    }

    SECTION("write")
    {
        auto static constexpr me = imat<3, 4>{1, 2, 3, 7,
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

TEST_CASE("matrix.submatrix")
{
    auto static constexpr base = imat<4, 5>{ 0,  1,  2,  3,  4,
                                            10, 11, 12, 13, 14,
                                            20, 21, 22, 23, 24,
                                            30, 31, 32, 33, 34};
    SECTION("single") {
        auto static const m1 = base.submatrix(2, 3);
        auto static const me = imat<3, 4>{ 0,  1,  2,  4,
                                          10, 11, 12, 14,
                                          30, 31, 32, 34};
        REQUIRE(m1 == me);
    }

    SECTION("multiple") {
        auto static const m1 = base.submatrix(1, 2, 1, 3);
        auto static const me = imat<2, 2>{ 0,  4,
                                          30, 34};
        REQUIRE(m1 == me);
    }
}

TEST_CASE("ext.det")
{
    SECTION("fs.1x1") {
        auto static constexpr m1 = imat<1, 1>{42};
        REQUIRE(det(m1) == 42);
    }

    SECTION("fs.2x2") {
        auto static constexpr m1 = imat<2, 2>{1, 2,
                                              3, 4};
        REQUIRE(det(m1) == -2);
    }

    SECTION("fs.3x3") {
        auto constexpr a = imat<3, 3>{3, 5, 1,
                                      1, 4, 2,
                                      7, 1, 9};
        auto constexpr d = det(a);
        REQUIRE(d == 100);
    }

    // SECTION("fs.4x4") {
    //     auto constexpr a = imat<4, 4>{4, 3, 2, 2,
    //                                   0, 1, 0,-2,
    //                                   1,-1, 0, 3,
    //                                   2, 3, 0, 1};
    //     auto constexpr d = det(a);
    //     REQUIRE(d == -10);
    // }
}

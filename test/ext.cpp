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
#include <sstream>

#include <catch2/catch.hpp>

TEST_CASE("ext.ostream.matrix")
{
    auto const m1 = imat<2, 3>{1, 2, 3,
                               4, 5, 6};
    auto os = std::ostringstream{};
    os << m1;
    CHECK(os.str() == "{{1, 2, 3}, {4, 5, 6}}");
}

TEST_CASE("ext.ostream.vector")
{
    auto const v1 = ivec<3>{1, 2, 3};
    auto os = std::ostringstream{};
    os << v1;
    CHECK(os.str() == "(1, 2, 3)");
}

TEST_CASE("ext.det")
{
    SECTION("fs.1x1") {
        auto static CONSTEXPR m1 = imat<1, 1>{42};
        REQUIRE(det(m1) == 42);
    }

    SECTION("fs.2x2") {
        auto static CONSTEXPR m1 = imat<2, 2>{1, 2,
                                              3, 4};
        REQUIRE(det(m1) == -2);
    }

    SECTION("fs.3x3") {
        auto CONSTEXPR a = imat<3, 3>{3, 5, 1,
                                      1, 4, 2,
                                      7, 1, 9};
        auto CONSTEXPR d = det(a);
        REQUIRE(d == 100);
    }

    // SECTION("fs.4x4") {
    //     auto CONSTEXPR a = imat<4, 4>{4, 3, 2, 2,
    //                                   0, 1, 0,-2,
    //                                   1,-1, 0, 3,
    //                                   2, 3, 0, 1};
    //     auto CONSTEXPR d = det(a);
    //     REQUIRE(d == -10);
    // }
}

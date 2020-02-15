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

TEST_CASE("multiplication: vector * vector")
{
    auto static constexpr v1 = ivec<3>{5, 2, 3};
    auto static constexpr v2 = ivec<3>{1, 2, 5};
    auto static constexpr v3 = v1 * v2;
    auto static constexpr expected = 24;
    REQUIRE(v3 == expected);
}

TEST_CASE("multiplication: scalar * vector")
{
    auto static constexpr v1 = ivec<3>{5, 2, 3};
    auto static constexpr s1 = 4;
    auto static constexpr v2 = s1 * v1;
    auto static constexpr expected = ivec<3>{20, 8, 12};
    REQUIRE(v2 == expected);
}

TEST_CASE("multiplication: vector * scalar")
{
    auto static constexpr v1 = ivec<3>{5, 2, 3};
    auto static constexpr s1 = 4;
    auto static constexpr v2 = v1 * s1;
    auto static constexpr expected = ivec<3>{20, 8, 12};
    REQUIRE(v2 == expected);
}

TEST_CASE("multiplication: scalar * matrix")
{
    auto static constexpr m1 = imat<2, 3>{1, 2, 3, 4, 5, 6};
    auto static constexpr s1 = 2;
    auto static constexpr m2 = s1 * m1;
    auto static constexpr expected = imat<2, 3>{2, 4, 6, 8, 10, 12};
    REQUIRE(m2 == expected);
}

TEST_CASE("multiplication: matrix * scalar")
{
    auto static constexpr m1 = imat<2, 3>{1, 2, 3, 4, 5, 6};
    auto static constexpr s1 = 2;
    auto static constexpr m2 = m1 * s1;
    auto static constexpr expected = imat<2, 3>{2, 4, 6, 8, 10, 12};
    REQUIRE(m2 == expected);
}

// TODO: matrix * matrix
// TODO: matrix * vector
// TODO: vector * matrix (where left hand side is treated as transposed vector, i.e. 1-row matrix)

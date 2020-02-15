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

TEST_CASE("subtraction: vector - vector")
{
    auto static const v1 = ivec<3>{5, 2, 1};
    auto static const v2 = ivec<3>{1, 2, 5};
    auto static const v3 = v1 - v2;
    auto static const expected = ivec<3>{4, 0, -4};
    REQUIRE(v3 == expected);
}

TEST_CASE("subtraction: matrix - matrix")
{
    auto static const m1 = imat<2, 2>{5, 2, 1, 0};
    auto static const m2 = imat<2, 2>{0, 2, 4, 8};
    auto static const m3 = m1 - m2;
    auto static const expected = imat<2, 2>{5, 0, -3, -8};
    REQUIRE(m3 == expected);
}

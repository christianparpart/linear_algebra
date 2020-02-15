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

TEST_CASE("negation: vector")
{
    auto static const v1 = ivec<3>{0, 1, 2};
    auto static const v2 = -v1;
    auto static const v3 = ivec<3>{0, -1, -2};
    REQUIRE(v2 == v3);
}

TEST_CASE("negation: matrix")
{
    auto static const m1 = imat<2, 2>{1, -2, 3, -4};
    auto static const m2 = -m1;
    auto static const m3 = imat<2, 2>{-1, 2, -3, 4};
    REQUIRE(m2 == m3);
}

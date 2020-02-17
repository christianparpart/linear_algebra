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

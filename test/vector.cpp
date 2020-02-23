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

TEST_CASE("vector.ctor")
{
    auto CONSTEXPR v = ivec<3>{2, 4, 8};
    REQUIRE(v.size() == 3);
    REQUIRE(v.elements() == 3);
    REQUIRE(v.capacity() == 3);

    // operator[]
    CHECK(v[0] == 2);
    CHECK(v[1] == 4);
    CHECK(v[2] == 8);

    // operator()
    CHECK(v(0) == 2);
    CHECK(v(1) == 4);
    CHECK(v(2) == 8);
}

TEST_CASE("vector.iterator")
{
    auto v = ivec<3>{2, 4, 8};
    auto i = v.begin();
    auto e = v.end();
    REQUIRE(*i == 2);
    i++;
    REQUIRE(*i == 4);
    i++;
    REQUIRE(*i == 8);
    i++;
    auto const iterationFinished = i == e;
    REQUIRE(iterationFinished != false);
}

TEST_CASE("vector.const_iterator")
{
    auto CONSTEXPR v = ivec<3>{2, 4, 8};
    auto i = v.cbegin();
    auto e = v.cend();
    REQUIRE(*i == 2);
    i++;
    REQUIRE(*i == 4);
    i++;
    REQUIRE(*i == 8);
    i++;
    auto const iterationFinished = i == e;
    REQUIRE(iterationFinished != false);
}

TEST_CASE("vector.reverse_iterator")
{
    auto v = ivec<3>{2, 4, 8};
    auto i = v.rbegin();
    auto e = v.rend();
    REQUIRE(*i == 8);
    i++;
    REQUIRE(*i == 4);
    i++;
    REQUIRE(*i == 2);
    i++;
    auto const iterationFinished = i == e;
    REQUIRE(iterationFinished != false);
}

TEST_CASE("vector.const_reverse_iterator")
{
    auto CONSTEXPR v = ivec<3>{2, 4, 8};
    auto i = v.crbegin();
    auto e = v.crend();
    REQUIRE(*i == 8);
    i++;
    REQUIRE(*i == 4);
    i++;
    REQUIRE(*i == 2);
    i++;
    auto const iterationFinished = i == e;
    REQUIRE(iterationFinished != false);
}

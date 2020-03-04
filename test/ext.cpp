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

namespace la = LINEAR_ALGEBRA_NAMESPACE;

TEST_CASE("ext.identity")
{
    SECTION("fs_matrix")
    {
        auto CONSTEXPR m1 = la::identity<int, 3>();
        CHECK(m1 == imat<3, 3>{1, 0, 0, 0, 1, 0, 0, 0, 1});
    }

    SECTION("dyn_matrix")
    {
        auto m1 = dmat<int>(math::identity<int, 3>());
        CHECK(m1 == imat<3, 3>{1, 0, 0, 0, 1, 0, 0, 0, 1});
    }
}

TEST_CASE("ext.trace")
{
    auto CONSTEXPR m1 = imat<3, 3>{ 1,   2,  4,
                                    8,  16,  32,
                                   64, 128, 256};
    CHECK(la::trace(m1) == 273);
}

TEST_CASE("ext.ostream.matrix")
{
    auto CONSTEXPR m1 = imat<2, 3>{1, 2, 3,
                                   4, 5, 6};
    auto os = std::ostringstream{};
    os << m1;
    CHECK(os.str() == "{{1, 2, 3}, {4, 5, 6}}");
}

TEST_CASE("ext.ostream.vector")
{
    auto CONSTEXPR v1 = ivec<3>{1, 2, 3};
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

    SECTION("fs.4x4") {
        auto CONSTEXPR a = imat<4, 4>{4, 3, 2, 2,
                                      0, 1, 0,-2,
                                      1,-1, 0, 3,
                                      2, 3, 0, 1};
        auto const d = det(a);
        REQUIRE(d == -10);
    }
}

// TEST_CASE("ext.inverse") // TODO
// {
//     auto CONSTEXPR static m = imat<3, 3>{1, 2, 3,
//                                          0, 1, 4,
//                                          5, 6, 0};
//     REQUIRE(is_invertible(m));
//     auto const mi = la::inverse(m);
//     CHECK(mi == imat<3, 3>{-24,  18,  5,
//                             20, -15, -4,
//                             -5,  4,  1});
// }

TEST_CASE("ext.permutation.identity")
{
    auto CONSTEXPR pi = la::permutation<3>::identity();
    std::cout << "pi: " << la::simple_form(pi) << '\n';
    std::cout << "pi: " << la::canonical_form(pi) << '\n';
    CHECK(pi(1) == 1);
    CHECK(pi(2) == 2);
    CHECK(pi(3) == 3);
    CHECK(la::simple_form(pi) == "(1 1) (2 2) (3 3)");
}

TEST_CASE("ext.permutation.transpositions")
{
    auto CONSTEXPR pi = la::permutation<6>{{5, 1}, {1, 2}, {2, 5}, {3, 4}, {4, 3}};
    auto const xi = la::canonical_form(pi);
    CHECK(xi == "(6) (5 1 2) (4 3)");
}

TEST_CASE("ext.permutation.all")
{
    auto const pa = la::permutation<3>::all();
    REQUIRE(pa.size() == 6);
    for (auto const& p : pa)
        std::cout << "p: " << p << '\n';
    // TODO: validate all
}

TEST_CASE("ext.permutation.composition")
{
    auto CONSTEXPR a = la::permutation<4>{2, 3, 1, 4};
    auto CONSTEXPR b = la::permutation<4>{3, 2, 4, 1};
    auto CONSTEXPR c = a * b;
    auto CONSTEXPR d = la::permutation<4>{1, 3, 4, 2};
    CHECK(c == d);
}

TEST_CASE("ext.permutation.inverse")
{
    auto CONSTEXPR a = la::permutation<4>{2, 3, 1, 4};
    auto CONSTEXPR b = la::inverse(a);
    auto CONSTEXPR c = a * b;
    auto CONSTEXPR I = la::permutation<4>::identity();
    CHECK(c == I);
}


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
#include "llsparse_matrix_engine.h"
#include <ostream>

#include <catch2/catch.hpp>

namespace la = LINEAR_ALGEBRA_NAMESPACE;

template <typename T>
using sparse_mat = la::matrix<llsparse_matrix_engine<T>, la::matrix_operation_traits>;

TEST_CASE("custom_engine.create")
{
    sparse_mat<int> const m1 = imat<2, 3>{1, 0, 2,
                                          0, 1, 0};
    auto static CONSTEXPR m2 = imat<2, 3>{1, 0, 2,
                                          0, 1, 0};
    CHECK(m1 == m2);
    CHECK(m1.engine().elements() == 3);
}

TEST_CASE("custom_engine.neg")
{
    auto const m1 = sparse_mat<int>(imat<2, 3>{1, 0, -2,
                                               0, -3, 0});
    auto static constexpr me = imat<2, 3>{-1, 0, 2,
                                           0, 3, 0};
    auto const r1 = -m1;
    CHECK(r1 == me);
}

TEST_CASE("custom_engine.add")
{
    sparse_mat<int> const m1 = imat<2, 3>{1, 0, 2,
                                          0, 1, 0};
    auto static CONSTEXPR m2 = imat<2, 3>{-1, 2, -2,
                                          4, -1, 0};
    auto static CONSTEXPR me = imat<2, 3>{0, 2, 0,
                                          4, 0, 0};
    auto const r1 = m1 + m2;
    CHECK(r1 == me);
    auto const r2 = m2 + m1;
    CHECK(r2 == me);
}

TEST_CASE("custom_engine.sub")
{
    auto const m1 = sparse_mat<int>(imat<2, 3>{1, 0, 2,
                                               0, 1, 0});
    auto const static m2 = imat<2, 3>{1, -2, 2,
                                      -4, 1, 0};

    // AB
    auto const static me = imat<2, 3>{0, 2, 0,
                                      4, 0, 0};
    auto const r1 = m1 - m2;
    CHECK(r1 == me);

    // BA
    auto const static me2 = imat<2, 3>{0, -2, 0,
                                       -4, 0, 0};
    auto const r2 = m2 - m1;
    CHECK(r2 == me2);
}

TEST_CASE("custom_engine.mul")
{
    // TODO
}

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

#pragma once

#include <version>
#include <cstdint>

#if !defined(LINEAR_ALGEBRA_CONCEPTS) && defined(__cpp_constexpr) && (__cpp_concepts >= 201507)
#define LA_CONCEPTS 1
#define LA_CONCEPT(X) requires(X)
#else
#define LA_CONCEPT(X) /*!*/
#endif

namespace dim {

#if defined(LINEAR_ALGEBRA_CONCEPTS)

// ring (R, +, *)
template <typename T>
concept Ring = requires(T a, T b) {
    ( a + b ) -> T;
    ( a - b ) -> T;

    ( a * b ) -> T;
    ( a / b ) -> T;

    ( -b ) -> T;

    // actually: ``-> bool`, but that didn't work?
    ( a == b );
    ( a != b );
    ( a <= b );
    ( a >= b );
    ( a < b );
    ( a > b );
};

// field (F, +, *)
template <typename T>
concept Field = requires(T a, T b) {
    ( a + b ) -> T;
    ( a - b ) -> T;

    ( a * b ) -> T;
    ( a / b ) -> T;

    ( -b ) -> T;

    // actually: ``-> bool`, but that didn't work?
    ( a == b );
    ( a != b );
    ( a <= b );
    ( a >= b );
    ( a < b );
    ( a > b );
};

template <typename F, typename Mat>
concept Matrix = Field<F> && Ring<Mat> && requires(F scalar, Mat mat, std::size_t i, std::size_t j) {
    { mat.row_count } -> std::size_t;
    { mat.column_count } -> std::size_t;
    { mat(i, j) } -> F;
    { scalar * mat } -> Mat;
    { mat * scalar } -> Mat;
};

#endif

} // end namespace

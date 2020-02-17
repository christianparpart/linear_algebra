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

#include "base.h"

#include "negation_traits.h"
#include "addition_traits.h"
#include "subtraction_traits.h"
#include "multiplication_traits.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

// ---------------------------------------------------------------------------

// Default matrix operation traits for
// - element promotion,
// - engine promotion, and
// - arithmetic operation traits for the four basic arithmetic operations.
struct matrix_operation_traits
{
    //- Default element promotion traits.
    //
    template <class T1>
    using element_negation_traits = matrix_negation_element_traits<T1>;
    template <class T1, class T2>
    using element_addition_traits = matrix_addition_element_traits<T1, T2>;
    template <class T1, class T2>
    using element_subtraction_traits = matrix_subtraction_element_traits<T1, T2>;
    template <class T1, class T2>
    using element_multiplication_traits = matrix_multiplication_element_traits<T1, T2>;

    //- Default engine promotion traits.
    //
    template <class OTR, class ET1>
    using engine_negation_traits = matrix_negation_engine_traits<OTR, ET1>;
    template <class OTR, class ET1, class ET2>
    using engine_addition_traits = matrix_addition_engine_traits<OTR, ET1, ET2>;
    template <class OTR, class ET1, class ET2>
    using engine_subtraction_traits = matrix_subtraction_engine_traits<OTR, ET1, ET2>;
    template <class OTR, class ET1, class ET2>
    using engine_multiplication_traits = matrix_multiplication_engine_traits<OTR, ET1, ET2>;

    //- Default arithmetic operation traits.
    //
    template <class OTR, class OP1> // XXX:DEFECT?
    using negation_traits = matrix_negation_traits<OTR, OP1>; // XXX:DEFECT
    template <class OTR, class OP1, class OP2>
    using addition_traits = matrix_addition_traits<OTR, OP1, OP2>;
    template <class OTR, class OP1, class OP2>
    using subtraction_traits = matrix_subtraction_traits<OTR, OP1, OP2>;
    template <class OTR, class OP1, class OP2>
    using multiplication_traits = matrix_multiplication_traits<OTR, OP1, OP2>;
};

// ---------------------------------------------------------------------------
// 6.6.2 | matrix_operation_traits_selector
// Primary template and expected specializations.

template <class T1, class T2>
struct matrix_operation_traits_selector;

template <class T1>
struct matrix_operation_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template <class T1>
struct matrix_operation_traits_selector<T1, matrix_operation_traits>
{
    using traits_type = T1;
};

template <class T1>
struct matrix_operation_traits_selector<matrix_operation_traits, T1>
{
    using traits_type = T1;
};

template <>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

//- Convenience alias.
template <class T1, class T2>
using matrix_operation_traits_selector_t = typename matrix_operation_traits_selector<T1, T2>::traits_type;

} // end namespace

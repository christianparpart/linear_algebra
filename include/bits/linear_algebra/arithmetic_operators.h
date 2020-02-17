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

#include "vector.h"
#include "matrix.h"
#include "operation_traits.h"

// 6.10 | arithmetic operators

namespace LINEAR_ALGEBRA_NAMESPACE {

//- Negation
//
template <class ET1, class OT1>
inline auto
constexpr operator-(vector<ET1, OT1> const& v1)
{
    using op1_type = vector<ET1, OT1>;
    using op_traits = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;
    return neg_traits::negate(v1);
}

template <class ET1, class OT1>
inline auto
constexpr operator-(matrix<ET1, OT1> const& m1)
{
    using op1_type = matrix<ET1, OT1>;
    using op_traits = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;
    return neg_traits::negate(m1);
}

//- Addition
//
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator+(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;
    return add_traits::add(v1, v2);
}

template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator+(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    static_assert(is_matrix_engine_v<ET1>);
    static_assert(is_matrix_engine_v<ET2>);

    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;
    return add_traits::add(m1, m2);
}

//- Subtraction
//
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator-(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;
    return sub_traits::subtract(v1, v2);
}

template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator-(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;
    return sub_traits::subtract(m1, m2);
}

//- Multiplication
//- vector*scalar and scalar*vector
//
template <class ET1, class OT1, class S2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);
    using op_traits = OT1;
    using op1_type = vector<ET1, OT1>;
    using op2_type = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, s2);
}

template <class S1, class ET2, class OT2>
inline auto
constexpr operator*(S1 const& s1, vector<ET2, OT2> const& v2)
{
    static_assert(is_matrix_element_v<S1>);
    using op_traits = OT2;
    using op1_type = S1;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(s1, v2);
}

// matrix*scalar and scalar*matrix
template <class ET1, class OT1, class S2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);
    using op_traits = OT1;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, s2);
}

template <class S1, class ET2, class OT2>
inline auto
constexpr operator*(S1 const& s1, matrix<ET2, OT2> const& m2)
{
    static_assert(is_matrix_element_v<S1>);
    using op_traits = OT2;
    using op1_type = S1;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(s1, m2);
}

// vector*vector
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, v2);
}

// matrix*vector
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, v2);
}

// vector*matrix
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, m2);
}

// matrix*matrix
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, m2);
}

} // end namespace

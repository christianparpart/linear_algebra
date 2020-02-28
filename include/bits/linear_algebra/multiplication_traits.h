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
#include <iostream>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.6.1 | matrix_operation_traits
template <class T1, class T2>
struct matrix_multiplication_element_traits
{
    using element_type = decltype(std::declval<T1>() * std::declval<T2>());
};

template <class OT, class T1, class T2>
using matrix_multiplication_element_t = typename OT::template element_multiplication_traits<T1, T2>::element_type;

// 6.8.4 | engine promotion traits | matrix_multiplication_engine_traits<OT, ET1, ET2>
template <class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits
/*{
    using element_type = matrix_multiplication_element_t<OT, typename ET1::element_type, typename ET2::element_type>;
    using engine_type = ET1; // TODO
}*/;

template <class OT, class T1, class T2, std::size_t N>
struct matrix_multiplication_engine_traits<OT, fs_vector_engine<T1, N>, fs_vector_engine<T2, N>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_vector_engine<matrix_multiplication_element_t<OT, T1, T2>, N>;
};

template <class OT, class T1, class T2, std::size_t N2>
struct matrix_multiplication_engine_traits<OT, scalar_engine<T1>, fs_vector_engine<T2, N2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_vector_engine<matrix_multiplication_element_t<OT, T1, T2>, N2>;
};

template <class OT, class T1, std::size_t N1, class T2>
struct matrix_multiplication_engine_traits<OT, fs_vector_engine<T1, N1>, scalar_engine<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_vector_engine<matrix_multiplication_element_t<OT, T1, T2>, N1>;
};

template <class OT, class T1, class T2, std::size_t R2, std::size_t C2>
struct matrix_multiplication_engine_traits<OT, scalar_engine<T1>, fs_matrix_engine<T2, R2, C2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_matrix_engine<matrix_multiplication_element_t<OT, T1, T2>, R2, C2>;
};

template <class OT, class T1, std::size_t R1, std::size_t C1, class T2>
struct matrix_multiplication_engine_traits<OT, fs_matrix_engine<T1, R1, C1>, scalar_engine<T2>>
{
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_matrix_engine<matrix_multiplication_element_t<OT, T1, T2>, R1, C1>;
};

template <class OT, class T1, std::size_t R1, std::size_t C1, class T2, std::size_t N2>
struct matrix_multiplication_engine_traits<OT, fs_matrix_engine<T1, R1, C1>, fs_vector_engine<T2, N2>>
{
    static_assert(C1 == N2, "Matrix-vector multiplication: matrix column count must equal vector element count.");
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_vector_engine<matrix_multiplication_element_t<OT, T1, T2>, R1>;
};

template <class OT, class T1, std::size_t R1, std::size_t C1, class T2, std::size_t R2, std::size_t C2>
struct matrix_multiplication_engine_traits<OT, fs_matrix_engine<T1, R1, C1>, fs_matrix_engine<T2, R2, C2>>
{
    static_assert(C1 == R2, "Matrix-matrix multiplication: left matrix column count must equal right matrix row count.");
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_matrix_engine<matrix_multiplication_element_t<OT, T1, T2>, R1, C2>;
};

// (fs * transpose)
template <typename OT,
          typename T1, std::size_t R1, std::size_t C1,
          typename T2, typename MCT2, std::size_t R2, std::size_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           fs_matrix_engine<T1, R1, C1>,
                                           transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    static_assert(C1 == C2, "Matrix-matrix multiplication: left matrix column count must equal right matrix row count.");
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_matrix_engine<element_type, R1, R2>;
};

// (transpose(fs) * transpose(fs))
template <typename OT,
          typename T1, typename MCT1, std::size_t R1, std::size_t C1,
          typename T2, typename MCT2, std::size_t R2, std::size_t C2>
struct matrix_multiplication_engine_traits<OT,
                                           transpose_engine<fs_matrix_engine<T1, R1, C1>, MCT1>,
                                           transpose_engine<fs_matrix_engine<T2, R2, C2>, MCT2>>
{
    static_assert(R1 == C2, "Matrix-matrix multiplication: left matrix column count must equal right matrix row count.");
    using element_type = matrix_multiplication_element_t<OT, T1, T2>;
    using engine_type = fs_matrix_engine<element_type, C1, R2>;
};

template <class OT, class ET1, class ET2>
using matrix_multiplication_engine_t =
    typename OT::template engine_multiplication_traits<
        OT, // TODO: OTR
        ET1,
        ET2
    >::engine_type;

// 6.9.4 matrix_multiplication_traits<OT, OP1, OP2>
template <class OT, class OP1, class OP2> struct matrix_multiplication_traits;

// vector * scalar
template <class OT, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, T2>
{
    using scalar_type = scalar_engine<T2>;
    using engine_type = matrix_multiplication_engine_t<OT, ET1, scalar_type>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(vector<ET1, OT1> const& v1, T2 const& s2)
    {
        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(v1.size());

        using detail::times;
        using detail::for_each;
        for_each(times(v1.size()), [&](auto i) constexpr { r(i) = v1(i) * s2; });

        return r;
    }
};

// scalar * vector
template <class OT, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, T1, vector<ET2, OT2>>
{
    using scalar_type = scalar_engine<T1>;
    using engine_type = matrix_multiplication_engine_t<OT, scalar_type, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(T1 const& s1, vector<ET2, OT2> const& v2)
    {
        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(v2.size());

        using detail::times;
        using detail::for_each;
        for_each(times(v2.size()), [&](auto i) constexpr { r(i) = s1 * v2(i); });

        return r;
    }
};

// matrix * scalar
template <class OT, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, T2>
{
    using scalar_type = scalar_engine<T2>;
    using engine_type = matrix_multiplication_engine_t<OT, ET1, scalar_type>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, T2 const& s2)
    {
        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(m1.size());

        using detail::times;
        for (auto [i, j] : times(r.rows()) * times(r.columns()))
            r(i, j) = m1(i, j) * s2;

        return r;
    }
};

// scalar * matrix
template <class OT, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, T1, matrix<ET2, OT2>>
{
    using scalar_type = scalar_engine<T1>;
    using engine_type = matrix_multiplication_engine_t<OT, scalar_type, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(T1 const& s1, matrix<ET2, OT2> const& m2)
    {
        return matrix_multiplication_traits<OT, matrix<ET2, OT2>, T1>::multiply(m2, s1);
        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(m2.size());

        using detail::times;
        for (auto [i, j] : times(r.rows()) * times(r.columns()))
            r(i, j) = s1 * m2(i, j);

        return r;
    }
};

// vector * vector
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    //- This specialization returns a scalar, and therefore does not compute an engine type.
    using op_traits = OT;
    using elem_type_1 = typename vector<ET1, OT1>::element_type;
    using elem_type_2 = typename vector<ET2, OT2>::element_type;
    using result_type = matrix_multiplication_element_t<op_traits, elem_type_1, elem_type_2>;
    constexpr static result_type multiply(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
    {
        using detail::times;
        using detail::reduce;
        return reduce(times(v1.size()), result_type{}, [&](auto acc, auto i) constexpr { return acc + v1(i) * v2(i); });
    }
};

// matrix * vector
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& m2)
    {
        using detail::reduce;
        using detail::times;

        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(m1.columns());

        for (auto i : times(m1.rows()))
            r(i) = reduce(times(m1.columns()), 0, [&](auto acc, auto j) { return acc + m1(i, j) * m2(j); });

        return r;
    }
};

// vector * matrix
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(vector<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        if constexpr (is_fixed_size_engine_v<engine_type>)
            static_assert(m1.size() == m2.rows(),
                          "Left-hand-side vector element count must match right-hand-side matrix row count.");

        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(m2.columns());

        using detail::times;
        using detail::reduce;

        for (auto j : times(m2.columns()))
            r(j) = detail::reduce(detail::times(m2.rows(), 0, [&, j=j](auto acc, auto i) {
                return acc + m1(i) * m2(i, j);
            }));

        return r;
    }
};

// matrix*matrix
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        result_type r;

        if constexpr (is_resizable_engine_v<engine_type>)
            r.resize(m1.rows(), m2.columns());

        using detail::times;
        using detail::reduce;
        using value_type = typename engine_type::value_type;

        for (auto [i, j] : times(r.rows()) * times(r.columns()))
            r(i, j) = reduce(times(m1.columns()), value_type{}, [&, i = i, j = j](auto acc, auto k) {
                return acc + m1(i, k) * m2(k, j);
            });

        return r;
    }
};

template <class OT, class OP1, class OP2>
using matrix_multiplication_traits_t =
    typename OT::template multiplication_traits<
        OT, // TODO: OTR!
        OP1,
        OP2
    >;

} // end namespace

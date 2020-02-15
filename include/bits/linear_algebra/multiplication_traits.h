#pragma once

#include "base.h"
#include "fwd.h"
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
        result_type r(v1.size());
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            r(i) = v1(i) * s2;
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
        result_type r(v2.size());
        for (decltype(v2.size()) i = 0; i < v2.size(); ++i)
            r(i) = s1 * v2(i);
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
        result_type r(m1.size());
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
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
        result_type r(m2.size());
        for (decltype(m2.rows()) i = 0; i < m2.rows(); ++i)
            for (decltype(m2.columns()) j = 0; j < m2.columns(); ++j)
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
        result_type r{};
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            r += v1(i) * v2(i);
        return r;
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
        result_type r(m1.columns());
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
        {
            typename engine_type::value_type bi{};
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                bi += m1(i, j) * m2(j);
            r(i) = bi;
        }
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
        // assert(m1.size() == m2.rows());
        result_type r(m2.columns());
        for (decltype(m2.columns()) j = 0; j < m2.columns(); ++j)
        {
            typename engine_type::value_type bi{};
            for (decltype(m2.rows()) i = 0; i < m2.rows(); ++i)
                bi += m1(i) * m2(i, j);
            r(j) = bi;
        }
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
        result_type r(m1.rows(), m2.columns());
        for (decltype(r.rows()) i = 0; i < r.rows(); ++i)
        {
            for (decltype(r.columns()) j = 0; j < r.columns(); ++j)
            {
                typename engine_type::value_type v{};
                for (decltype(m1.columns()) k = 0; k < m1.columns(); ++k)
                    v += m1(i, k) * m2(k, j);
                r(i, j) = v;
            }
        }
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

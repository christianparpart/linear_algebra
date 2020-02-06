// vim:ts=4:et
#pragma once

#include <cstdint>
#include <algorithm>
#include <complex>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <type_traits>

namespace math {

//- Tags that describe engines and their capabilities.
//
struct scalar_engine_tag {};
struct readable_vector_engine_tag {};
struct writable_vector_engine_tag {};
struct resizable_vector_engine_tag {};
struct readable_matrix_engine_tag {};
struct writable_matrix_engine_tag {};
struct resizable_matrix_engine_tag {};

//- Owning engines with dynamically-allocated external storage.
//
template<class T, class AT> class dr_vector_engine;
template<class T, class AT> class dr_matrix_engine;

//- Owning engines with fixed-size internal storage.
//
template<class T, size_t N> class fs_vector_engine;
template<class T, size_t R, size_t C> class fs_matrix_engine;

//- Non-owning, view-style engines.
//
template<class ET, class VCT> class column_engine;
template<class ET, class VCT> class row_engine;
template<class ET, class MCT> class transpose_engine;
template<class ET, class MCT> class submatrix_engine;

//- Non-owning dummy engine to represent scalar operands.
//
template<class T> struct scalar_engine;

//- The default element promotion, engine promotion, and arithmetic operation
// traits for the four basic arithmetic operations.
//
struct matrix_operation_traits;

//- Primary math object types.
//
template<class ET, class OT=matrix_operation_traits> class vector;
template<class ET, class OT=matrix_operation_traits> class matrix;

//- Math object element promotion traits, per arithmetical operation.
//
template<class T1> struct matrix_negation_element_traits;
template<class T1, class T2> struct matrix_addition_element_traits;
template<class T1, class T2> struct matrix_subtraction_element_traits;
template<class T1, class T2> struct matrix_multiplication_element_traits;

//- Math object engine promotion traits, per arithmetical operation.
//
template<class OT, class ET1> struct matrix_negation_engine_traits;
template<class OT, class ET1, class ET2> struct matrix_addition_engine_traits;
template<class OT, class ET1, class ET2> struct matrix_subtraction_engine_traits;
template<class OT, class ET1, class ET2> struct matrix_multiplication_engine_traits;

//- Math object arithmetic traits, per arithmetical operation.
//
template<class OT, class OP1> struct matrix_negation_traits;
template<class OT, class OP1, class OP2> struct matrix_addition_traits;
template<class OT, class OP1, class OP2> struct matrix_subtraction_traits;
template<class OT, class OP1, class OP2> struct matrix_multiplication_traits;

//- A traits type that chooses between two operation traits types in the binary
// arithmetic operators and free functions that act like binary operators.
// This traits class is a customization point.
//
template<class OT1, class OT2> struct matrix_operation_traits_selector;

//- Addition operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator +(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
template<class ET1, class OT1, class ET2, class OT2>
auto operator +(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Subtraction operators
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator -(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);
template<class ET1, class OT1, class ET2, class OT2>
auto operator -(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Negation operators
//
template<class ET1, class OT1>
auto operator -(vector<ET1, OT1> const& v1);
template<class ET1, class OT1, class ET2, class OT2>
auto operator -(matrix<ET1, OT1> const& m1);

//- Vector*Scalar multiplication operators
//
template<class ET1, class OT1, class S2>
auto operator *(vector<ET1, OT1> const& v1, S2 const& s2);
template<class S1, class ET2, class OT2>
auto operator *(S1 const& s1, vector<ET2, OT2> const& v2);

//- Matrix*Scalar multiplication operators
//
template<class ET1, class OT1, class S2>
auto operator *(matrix<ET1, OT1> const& m1, S2 const& s2);
template<class S1, class ET2, class OT2>
auto operator *(S1 const& s1, matrix<ET2, OT2> const& m2);

//- Vector*Matrix multiplication operator
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator *(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2);

//- Matrix*Vector multiplication operator
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator *(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2);

//- Vector*Vector multiplication operator
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator *(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2);

//- Matrix*Matrix multiplication operator
//
template<class ET1, class OT1, class ET2, class OT2>
auto operator *(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2);

//- Convenience aliases for vector and matrix objects based on
// dynamically-resizable engines.
//
template<class T, class AT = std::allocator<T>>
using dyn_vector = vector<dr_vector_engine<T, AT>, matrix_operation_traits>;
template<class T, class AT = std::allocator<T>>
using dyn_matrix = matrix<dr_matrix_engine<T, AT>, matrix_operation_traits>;

//- Convenience aliases for vector and matrix objects based on fixed-size engines.
//
template<class T, int32_t N>
using fs_vector = vector<fs_vector_engine<T, N>, matrix_operation_traits>;
template<class T, int32_t R, int32_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>, matrix_operation_traits>;

} // end namespace

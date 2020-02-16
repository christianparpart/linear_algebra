#pragma once

#include "defs.h"
#include "support.h"
#include <type_traits>

/**
 * - Parameter names T, T1, and T2 represent element types.
 * — Parameter names ET, ET1, and ET2 represent engine types.
 * — Parameter names OT, OT1, and OT2 represent operation traits types.
 * — Parameter names OP, OP1, and OP2 represent the operand types deduced by an arithmetic operator.
 * — Parameter names AT, AT1, and AT2 represent allocator types.
 * — Parameter names N, N1, and N2 represent the number of elements in a fixed-size vector.
 * — Parameter names C, C1, and C2 represent the number of columns in a fixed-size matrix.
 * — Parameter names R, R1, and R2 represent the number of rows in a fixed-size matrix.
 * — Parameter name VCT represents a vector engine’s category tag type.
 * — Parameter name MCT represents a matrix engine’s category tag type.
 */

namespace LINEAR_ALGEBRA_NAMESPACE {

using TODO = void*; // XXX well, yeah ...

//- Tags that describe engines and their capabilities.
//
struct scalar_engine_tag {};
struct readable_vector_engine_tag {};   // i.e. vector read-only
struct writable_vector_engine_tag {};   // i.e. vector read-write fixed-size
struct resizable_vector_engine_tag {};  // i.e. vector read-write resizable
struct readable_matrix_engine_tag {};   // i.e. matrix read-only
struct writable_matrix_engine_tag {};   // i.e. matrix read-write fixed-size
struct resizable_matrix_engine_tag {};  // i.e. matrix read-write resizable

template <class ET> constexpr inline bool is_scalar_engine_v =
    std::is_same_v<typename ET::engine_category, scalar_engine_tag>;

template <class ET> constexpr inline bool is_readonly_engine_v =
    std::is_same_v<typename ET::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, readable_vector_engine_tag>;

template <class ET> constexpr inline bool is_writable_engine_v =
    std::is_same_v<typename ET::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_vector_engine_tag>;

template <class ET> constexpr inline bool is_resizable_engine_v =
    std::is_same_v<typename ET::engine_category, resizable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_vector_engine_tag>;

template <class ET> constexpr inline bool is_matrix_engine_v =
    std::is_same_v<typename ET::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_matrix_engine_tag>;

template <class VCT> constexpr inline bool is_vector_engine_tag =
    std::is_same_v<VCT, readable_vector_engine_tag> ||
    std::is_same_v<VCT, writable_vector_engine_tag> ||
    std::is_same_v<VCT, resizable_vector_engine_tag>;

template <class T> constexpr inline bool is_matrix_element_v =
    std::is_arithmetic_v<T> ||
    detail::is_complex_v<T>;

} // end namespace

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

// Trivial engine that represents the scalar operand.
template <typename T> struct scalar_engine;

// Owning fixed-size engines.
template <typename T, size_t N> class fs_vector_engine;
template <typename T, size_t R, size_t C> class fs_matrix_engine;

// Owning engines with dynamically-allocated external storage.
template <typename T, typename AT = std::allocator<T>> class dr_vector_engine;
template <typename T, typename AT = std::allocator<T>> class dr_matrix_engine;

// Non-owning engines.
template <typename ET, typename VCT, typename VFT> class vector_view_engine;
template <typename ET, typename MCT, typename VFT> class matrix_view_engine;

struct subvector_view_tag;
struct column_view_tag;
struct row_view_tag;

struct submatrix_view_tag;
struct transpose_view_tag;

template <typename ET, typename VCT>
using subvector_engine = vector_view_engine<ET, VCT, subvector_view_tag>;

template <typename ET, typename VCT>
using column_engine = vector_view_engine<ET, VCT, column_view_tag>;

template <typename ET, typename VCT>
using row_engine = vector_view_engine<ET, VCT, row_view_tag>;

template <typename ET, typename MCT>
using submatrix_engine = matrix_view_engine<ET, MCT, submatrix_view_tag>;

template <typename ET, typename MCT>
using transpose_engine = matrix_view_engine<ET, MCT, transpose_view_tag>;

struct matrix_operation_traits;

template <typename ET, typename OT = matrix_operation_traits> class vector;
template <typename ET, typename OT = matrix_operation_traits> class matrix;

template <typename ET> constexpr inline bool is_scalar_engine_v =
    std::is_same_v<typename ET::engine_category, scalar_engine_tag>;

template <typename ET> constexpr inline bool is_readonly_engine_v =
    std::is_same_v<typename ET::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, readable_vector_engine_tag>;

template <typename ET> constexpr inline bool is_writable_engine_v =
    std::is_same_v<typename ET::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_vector_engine_tag>;

template <typename ET> constexpr inline bool is_resizable_engine_v =
    std::is_same_v<typename ET::engine_category, resizable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_vector_engine_tag>;

template <typename ET> constexpr inline bool is_fixed_size_engine_v = !is_resizable_engine_v<ET>;

template <typename ET> constexpr inline bool is_matrix_engine_v =
    std::is_same_v<typename ET::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_matrix_engine_tag>;

template <typename ET> constexpr inline bool is_vector_engine_v =
    std::is_same_v<typename ET::engine_category, readable_vector_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_vector_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_vector_engine_tag>;

template <typename ET> constexpr inline bool is_engine_v = is_matrix_engine_v<ET> || is_vector_engine_v<ET>;

template <typename ET> struct is_submatrix_engine : public std::false_type {};
template <typename T, size_t R, size_t C, typename MCT> struct is_submatrix_engine<matrix_view_engine<fs_matrix_engine<T, R, C>, MCT, submatrix_view_tag>> : public std::true_type {};
template <typename ET> constexpr inline bool is_submatrix_engine_v = is_submatrix_engine<ET>::value;
// TODO: rename to is_fs_submatrix_engine_v

template <typename VCT> constexpr inline bool is_vector_engine_tag =
    std::is_same_v<VCT, readable_vector_engine_tag> ||
    std::is_same_v<VCT, writable_vector_engine_tag> ||
    std::is_same_v<VCT, resizable_vector_engine_tag>;

template <typename T> constexpr inline bool is_matrix_element_v =
    std::is_arithmetic_v<T> ||
    detail::is_complex_v<T>;

// EXT: row_count_v<ET> evaluates to the number of rows of the given engine.
template <typename ET> struct row_count;
template <typename T, size_t R, size_t C> struct row_count<fs_matrix_engine<T, R, C>> { static constexpr size_t value = R; };
template <typename T, size_t N> struct row_count<fs_vector_engine<T, N>> { static constexpr size_t value = N; };
template <typename ET> constexpr inline size_t row_count_v = row_count<ET>::value;

// EXT: column_count_v<ET> evaluates to the number of columns of the given engine.
template <typename ET> struct column_count;
template <typename T, size_t R, size_t C> struct column_count<fs_matrix_engine<T, R, C>> { static constexpr size_t value = R; };
template <typename T, size_t N> struct column_count<fs_vector_engine<T, N>> { static constexpr size_t value = N; };
template <typename ET> constexpr inline size_t column_count_v = column_count<ET>::value;

// CRTP-style base class for matrix engines (well, and `matrix` class itself).
template <typename ET, typename MCT>
class matrix_engine {
  public:
    using engine_category = MCT;
    using size_type = std::size_t;

    //- Typed accessor to the actual engine type.
    //
    constexpr ET& engine() noexcept { return static_cast<ET&>(*this); }
    constexpr ET& engine() const noexcept { return static_cast<ET const&>(*this); }

    //- Capacity
    //
    constexpr auto columns() const noexcept { return engine().columns(); }
    constexpr auto rows() const noexcept { return engine().rows(); }
    constexpr auto size() const noexcept { return engine().size(); }
    constexpr auto column_capacity() const noexcept { return engine().column_capacity(); }
    constexpr auto row_capacity() const noexcept { return engine().row_capacity(); }
    constexpr auto capacity() const noexcept { return engine().capacity(); }

    //- Element access
    //
    constexpr decltype(auto) operator()(size_type i, size_type j) const { return engine()(i, j); }

    //- Modifiers
    //
    constexpr void swap(matrix_engine<ET, MCT>& rhs) { engine().swap(rhs); }
};

} // end namespace

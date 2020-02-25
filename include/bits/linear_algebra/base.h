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

template <typename ET, typename OT> class vector;
template <typename ET, typename OT> class matrix;

template <typename T, size_t N> class fs_vector_engine;
template <typename T, size_t R, size_t C> class fs_matrix_engine;

template <typename T> struct scalar_engine;
template <typename ET, typename OT> class row_engine;
template <typename ET, typename OT> class column_engine;
template <typename ET, typename OT> class transpose_engine;
template <typename ET, typename OT> class submatrix_engine;

struct matrix_operation_traits;

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

template <class ET> constexpr inline bool is_fixed_size_engine_v = !is_resizable_engine_v<ET>;

template <class ET> constexpr inline bool is_matrix_engine_v =
    std::is_same_v<typename ET::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_matrix_engine_tag>;

template <class ET> constexpr inline bool is_vector_engine_v =
    std::is_same_v<typename ET::engine_category, readable_vector_engine_tag> ||
    std::is_same_v<typename ET::engine_category, writable_vector_engine_tag> ||
    std::is_same_v<typename ET::engine_category, resizable_vector_engine_tag>;

template <typename ET> constexpr inline bool is_engine_v = is_matrix_engine_v<ET> || is_vector_engine_v<ET>;

template <class VCT> constexpr inline bool is_vector_engine_tag =
    std::is_same_v<VCT, readable_vector_engine_tag> ||
    std::is_same_v<VCT, writable_vector_engine_tag> ||
    std::is_same_v<VCT, resizable_vector_engine_tag>;

template <class T> constexpr inline bool is_matrix_element_v =
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
template <class ET, class MCT>
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

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
#include "concepts.h"
#include "column_engine.h"
#include "row_engine.h"
#include "submatrix_engine.h"
#include "transpose_engine.h"
#include "vector.h"

#include <type_traits>
#include <initializer_list>
#include <tuple>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.5.2 | class matrix<ET, OT>
template <class ET, class OT>
class matrix {
    static_assert(is_matrix_engine_v<ET>, "ET must obey the rules of a matrix engine type.");

    constexpr static bool inline is_resizable = is_resizable_engine_v<ET>;

    using engine_category = typename ET::engine_category;
    using equiv_vector_engine_tag = std::conditional_t<
            std::is_same_v<engine_category, readable_matrix_engine_tag>, readable_vector_engine_tag,
                                                                         writable_vector_engine_tag>;
    using as_writable_matrix_engine_tag = std::conditional_t<
            std::is_same_v<engine_category, readable_matrix_engine_tag>, readable_matrix_engine_tag,
                                                                         writable_matrix_engine_tag>;

  private:
    ET engine_{};

  public:
    //- Types
    //
    using engine_type = ET;
    using element_type = typename engine_type::element_type;
    using value_type = typename engine_type::value_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using difference_type = typename engine_type::difference_type;
    using size_type = typename engine_type::size_type;
    using size_tuple = typename engine_type::size_tuple;
    using column_type = vector<column_engine<ET, equiv_vector_engine_tag>, OT>;
    using const_column_type = vector<column_engine<ET, readable_vector_engine_tag>, OT>;
    using row_type = vector<row_engine<ET, equiv_vector_engine_tag>, OT>;
    using const_row_type = vector<row_engine<ET, readable_vector_engine_tag>, OT>;
    using submatrix_type = matrix<submatrix_engine<ET, as_writable_matrix_engine_tag>, OT>;
    using const_submatrix_type = matrix<submatrix_engine<ET, readable_matrix_engine_tag>, OT>;
    using transpose_type = matrix<transpose_engine<ET, as_writable_matrix_engine_tag>, OT>;
    using const_transpose_type = matrix<transpose_engine<ET, readable_matrix_engine_tag>, OT>;
    using hermitian_type = std::conditional_t<detail::is_complex_v<element_type>, matrix, transpose_type>;
    using const_hermitian_type = std::conditional_t<detail::is_complex_v<element_type>, matrix, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;
    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;
    template <typename U>
    constexpr matrix(std::initializer_list<U> list) : engine_(list) {}

    template <class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src)
    {
        using detail::times;
        if constexpr (is_resizable)
        {
            engine_.reserve(src.row_capacity(), src.column_capacity());
            engine_.resize(src.rows(), src.columns());
        }
        for (auto [i, j] : times(rows()) * times(columns()))
            (*this)(i, j) = src(i, j);
    }

    constexpr matrix(size_tuple size) LA_CONCEPT(is_resizable) { resize(size); }
    constexpr matrix(size_type rows, size_type cols) LA_CONCEPT(is_resizable) { resize(rows, cols); }

    constexpr matrix(size_tuple size, size_tuple cap) LA_CONCEPT(is_resizable)
    {
        reserve(cap);
        resize(size);
    }

    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap) LA_CONCEPT(is_resizable)
    {
        reserve(rowcap, colcap);
        resize(rows, cols);
    }

    constexpr explicit matrix(ET&& _engine) : engine_{std::forward<ET>(_engine)} {} // EXT

    // EXT
    template<
        typename Initializer,
        typename std::enable_if_t<
            std::is_invocable_r_v<value_type, Initializer, size_type, size_type>,
            int> = 0
    >
    constexpr explicit matrix(Initializer const& _init) noexcept
    {
        using detail::times;
        for (auto [i, j] : times(0, rows()) * times(0, columns()))
            (*this)(i, j) = _init(i, j);
    }

    // EXT
    template<
        typename Initializer,
        typename std::enable_if_t<
            std::is_invocable_r_v<value_type, Initializer, size_type, size_type>,
            int> = 0
    >
    constexpr matrix(size_type rows, size_type cols, Initializer const& _init) noexcept
        LA_CONCEPT(is_resizable)
        : matrix(rows, cols)
    {
        using detail::times;
        for (auto [i, j] : times(rows) * times(cols))
            (*this)(i, j) = _init(i, j);
    }

    constexpr matrix& operator=(matrix&&) noexcept(noexcept(std::is_nothrow_move_assignable_v<matrix>)) = default;
    constexpr matrix& operator=(matrix const&) noexcept(noexcept(std::is_nothrow_move_assignable_v<matrix>)) = default;

    template <class ET2, class OT2>
    constexpr matrix& operator=(matrix<ET2, OT2> const& rhs)
    {
        if constexpr (is_fixed_size_engine_v<engine_type>)
            static_assert(rows() == rhs.rows() && columns() == rhs.columns(), "Matrix dimensions must match.");

        if constexpr (is_resizable_engine_v<engine_type>)
            resize(rhs.size());

        using detail::times;
        for (auto [i, j] : times(rows()) * times(columns()))
            (*this)(i, j) = rhs(i, j);
    }

    //- Capacity
    //
    constexpr size_type rows() const noexcept { return engine_.rows(); }
    constexpr size_type columns() const noexcept { return engine_.columns(); }
    constexpr size_tuple size() const noexcept { return engine_.size(); }
    constexpr size_type column_capacity() const noexcept { return engine_.column_capacity(); }
    constexpr size_type row_capacity() const noexcept { return engine_.row_capacity(); }
    constexpr size_tuple capacity() const noexcept { return engine_.capacity(); }

    constexpr void reserve(size_tuple cap) LA_CONCEPT(is_resizable) { engine_.reserve(cap); }
    constexpr void reserve(size_type rowcap, size_type colcap) LA_CONCEPT(is_resizable) { engine_.reserve(rowcap, colcap); }
    constexpr void resize(size_tuple size) LA_CONCEPT(is_resizable) { engine_.resize(std::get<0>(size), std::get<1>(size)); }
    constexpr void resize(size_type rows, size_type cols) LA_CONCEPT(is_resizable) { engine_.resize(rows, cols); }
    constexpr void resize(size_tuple size, size_tuple cap) LA_CONCEPT(is_resizable) { engine_.resize(size, cap); }
    constexpr void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap) LA_CONCEPT(is_resizable) { engine_.resize(rows, cols, rowcap, colcap); }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) { return engine_(i, j); }
    constexpr const_reference operator()(size_type i, size_type j) const { return engine_(i, j); }

    //- Columns, rows, submatrices, transposes, and the Hermitian
    //
    constexpr column_type column(size_type j) noexcept { return column_type(typename column_type::engine_type(engine_, j)); }
    constexpr const_column_type column(size_type j) const noexcept { return const_column_type(typename const_column_type::engine_type(const_cast<ET&>(engine_), j)); }
    constexpr row_type row(size_type i) noexcept { return row_type(typename row_type::engine_type(engine_, i)); }
    constexpr const_row_type row(size_type i) const noexcept { return const_row_type(typename const_row_type::engine_type(const_cast<ET&>(engine_), i)); }
    constexpr submatrix_type submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept {
        return submatrix_type(typename submatrix_type::engine_type(const_cast<ET&>(engine_), ri, rn, ci, cn));
    }
    constexpr const_submatrix_type submatrix(size_type ri, size_type rn,
                                             size_type ci, size_type cn) const noexcept {
        return const_submatrix_type(typename const_submatrix_type::engine_type(const_cast<ET*>(&engine_), ri, rn, ci, cn));
    }
    constexpr submatrix_type submatrix(size_type r, size_type c) noexcept { // EXT
        return submatrix(r, 1, c, 1);
    }
    constexpr const_submatrix_type submatrix(size_type r, size_type c) const noexcept { // EXT
        return submatrix(r, 1, c, 1);
    }
    constexpr transpose_type t() noexcept
    {
        return transpose_type(typename transpose_type::engine_type(&engine_));
    }
    constexpr const_transpose_type t() const noexcept
    {
        return const_transpose_type(typename const_transpose_type::engine_type(const_cast<ET*>(&engine_)));
    }
    constexpr hermitian_type h(); // TODO
    constexpr const_hermitian_type h() const; // TODO

    //- Data access
    //
    constexpr engine_type& engine() noexcept { return engine_; }
    constexpr engine_type const& engine() const noexcept { return engine_; }

    //- Modifiers
    //
    constexpr void swap(matrix& rhs) noexcept { engine_.swap(rhs.engine_); }
    constexpr void swap_columns(size_type i, size_type j) noexcept LA_CONCEPT(!is_readonly_engine_v<engine_type>) { engine_.swap_columns(i, j); }
    constexpr void swap_rows(size_type i, size_type j) noexcept LA_CONCEPT(!is_readonly_engine_v<engine_type>) { engine_.swap_rows(i, j); }
};

} // end namespace

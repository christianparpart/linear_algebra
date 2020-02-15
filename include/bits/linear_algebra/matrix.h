#pragma once

#include "base.h"
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
    using column_type = vector<column_engine<ET, TODO /*see below*/ >, OT>;
    using const_column_type = vector<column_engine<ET, readable_vector_engine_tag>, OT>;
    using row_type = vector<row_engine<ET, TODO /*see below*/ >, OT>;
    using const_row_type = vector<row_engine<ET, readable_vector_engine_tag>, OT>;
    using submatrix_type = matrix<submatrix_engine<ET, TODO /*see below*/ >, OT>;
    using const_submatrix_type = matrix<submatrix_engine<ET, readable_matrix_engine_tag>, OT>;
    using transpose_type = matrix<transpose_engine<ET, TODO /*see below*/ >, OT>;
    using const_transpose_type = matrix<transpose_engine<ET, readable_matrix_engine_tag>, OT>;
    using hermitian_type = std::conditional_t<util::is_complex_v<element_type>, matrix, transpose_type>;
    using const_hermitian_type = std::conditional_t<util::is_complex_v<element_type>, matrix, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;
    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;
    template <typename U>
    constexpr matrix(std::initializer_list<U> list) : engine_(list) {}

    template <class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src) : engine_{src.engine_} {}
    constexpr matrix(size_tuple size) { resize(size); }
    constexpr matrix(size_type rows, size_type cols);
    constexpr matrix(size_tuple size, size_tuple cap);
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    constexpr matrix& operator=(matrix&&) noexcept = default;
    constexpr matrix& operator=(matrix const&) = default;
    template <class ET2, class OT2>
    constexpr matrix& operator=(matrix<ET2, OT2> const& rhs);

    //- Capacity
    //
    constexpr size_type rows() const noexcept { return engine_.rows(); }
    constexpr size_type columns() const noexcept { return engine_.columns(); }
    constexpr size_tuple size() const noexcept { return engine_.size(); }
    constexpr size_type column_capacity() const noexcept { return engine_.column_capacity(); }
    constexpr size_type row_capacity() const noexcept { return engine_.row_capacity(); }
    constexpr size_tuple capacity() const noexcept { return engine_.capacity(); }
    constexpr void reserve(size_tuple cap) { engine_.reserve(cap); }
    constexpr void reserve(size_type rowcap, size_type colcap) { engine_.reserve(rowcap, colcap); }
    constexpr void resize([[maybe_unused]] size_tuple size)
    {
        // static_assert(std::is_same_v<engine_type::engine_category, resizable_matrix_engine_tag>);
        // if constexpr (std::is_same_v<engine_type::engine_category, resizable_matrix_engine_tag>)
        //     engine_.resize(std::get<0>(size), std::get<1>(size));
    }
    constexpr void resize(size_type rows, size_type cols) { engine_.resize(rows, cols); }
    constexpr void resize(size_tuple size, size_tuple cap) { engine_.resize(size, cap); }
    constexpr void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    {
        engine_.resize(rows, cols, rowcap, colcap);
    }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) { return engine_(i, j); }
    constexpr const_reference operator()(size_type i, size_type j) const { return engine_(i, j); }

    //- Columns, rows, submatrices, transposes, and the Hermitian
    //
    constexpr column_type column(size_type j) noexcept { return engine_.column(j); }
    constexpr const_column_type column(size_type j) const noexcept { return engine_.column(j); }
    constexpr row_type row(size_type i) noexcept { return engine_.row(i); }
    constexpr const_row_type row(size_type i) const noexcept { return engine_.row(i); }
    constexpr submatrix_type submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept {
        return engine_.submatrix(ri, rn, ci, cn);
    }
    constexpr const_submatrix_type submatrix(size_type ri, size_type rn,
                                             size_type ci, size_type cn) const noexcept {
        return engine_.submatrix(ri, rn, ci, cn);
    }
    constexpr transpose_type t() noexcept; // TODO
    constexpr const_transpose_type t() const noexcept; // TODO
    constexpr hermitian_type h(); // TODO
    constexpr const_hermitian_type h() const; // TODO

    //- Data access
    //
    constexpr engine_type& engine() noexcept { return engine_; }
    constexpr engine_type const& engine() const noexcept { return engine_; }

    //- Modifiers
    //
    constexpr void swap(matrix& rhs) noexcept { engine_.swap(rhs.engine_); }
    constexpr void swap_columns(size_type i, size_type j) noexcept { engine_.swap_columns(i, j); }
    constexpr void swap_rows(size_type i, size_type j) noexcept { engine_.swap_rows(i, j); }
};

} // end namespace

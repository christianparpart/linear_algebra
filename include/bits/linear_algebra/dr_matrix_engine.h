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

#include <cassert>
#include <vector>
#include <tuple>
#include <type_traits>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.2
template<class T, class AT>
class dr_matrix_engine
{
  public:
    //- Types
    //
    using engine_category = resizable_matrix_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using allocator_type = AT;
    using pointer = typename std::allocator_traits<AT>::pointer;
    using const_pointer = typename std::allocator_traits<AT>::const_pointer;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using size_tuple = std::tuple<size_type, size_type>;

    //- Construct/copy/destroy
    //
    ~dr_matrix_engine() noexcept = default;
    dr_matrix_engine() = default;
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept = default;
    dr_matrix_engine(dr_matrix_engine const& rhs) = default;
    dr_matrix_engine(size_type rows, size_type cols) : dr_matrix_engine(rows, rows, cols, cols) {}
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    {
        assert(rows <= rowcap && cols <= colcap);

        rows_ = rows;
        columns_ = cols;
        row_capacity_ = rowcap;
        column_capacity_ = colcap;

        elements_.resize(rowcap * colcap);
    }
    dr_matrix_engine& operator=(dr_matrix_engine&&) noexcept = default;
    dr_matrix_engine& operator=(dr_matrix_engine const&) = default;
    template<class ET2>
    dr_matrix_engine& operator=(ET2 const& rhs)
    {
        dr_matrix_engine clone(rhs);
        clone.swap(*this);
        return *this;
    }

    //- Capacity
    //
    size_type columns() const noexcept { return columns_; }
    size_type rows() const noexcept { return rows_; }
    size_tuple size() const noexcept { return {rows(), columns()}; }
    size_type column_capacity() const noexcept { return column_capacity_; }
    size_type row_capacity() const noexcept { return row_capacity_; }
    size_tuple capacity() const noexcept { return {row_capacity(), column_capacity()}; }
    void reserve(size_type rowcap, size_type colcap)
    {
        using detail::times;
        dr_matrix_engine resized(rows(), columns(), rowcap, colcap);
        for (auto [i, j] : times(rows()) * times(columns()))
            resized(i, j) = (*this)(i, j);
        std::swap(resized, *this);
    }
    void resize(size_type rows, size_type cols)
    {
        if (rows > row_capacity_ || cols > column_capacity_)
            reserve(rows, cols);

        rows_ = rows;
        columns_ = cols;
    }
    void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    {
        reserve(rowcap, colcap);
        resize(rows, cols);
    }

    //- Element access
    //
    reference operator()(size_type i, size_type j) { return elements_[i * column_capacity() + j]; }
    const_reference operator()(size_type i, size_type j) const { return elements_[i * column_capacity() + j]; }

    //- Modifiers
    //
    void swap(dr_matrix_engine& other) noexcept
    {
        std::swap(elements_, other.elements_);
    }

    void swap_columns(size_type c1, size_type c2) noexcept
    {
        for (auto r : detail::times(rows()))
            std::swap((*this)(r, c1), (*this)(r, c2));
    }

    void swap_rows(size_type r1, size_type r2) noexcept
    {
        for (auto c : detail::times(columns()))
            std::swap((*this)(r1, c), (*this)(r2, c));
    }

  private:
    std::vector<T, AT> elements_;
    size_type row_capacity_;
    size_type column_capacity_;
    size_type rows_;
    size_type columns_;
};

} // end namespace

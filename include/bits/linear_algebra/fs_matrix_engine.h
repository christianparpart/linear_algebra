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

#include <array>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <utility>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.4 | class fs_matrix_engine<T, R, C>
template <class T, size_t R, size_t C>
class fs_matrix_engine : public matrix_engine<fs_matrix_engine<T, R, C>, writable_matrix_engine_tag>
{
    static_assert(R >= 1 && C >= 1, "Row and column count must be at least one.");
    static_assert(is_matrix_element_v<T>, "Element type must be an arithmetic field type.");

  public:
    //- Types
    //
    using engine_category = writable_matrix_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using size_tuple = std::tuple<size_type, size_type>;

    //- Construct/copy/destroy
    //
    ~fs_matrix_engine() noexcept = default;
    constexpr fs_matrix_engine() = default;

    template <typename U>
    constexpr fs_matrix_engine(std::initializer_list<U> _values)
    {
        size_t i = 0;
        for (auto v : _values)
            values_[i++] = v;
        //TODO(constexpr) std::copy(std::begin(_values), std::end(_values), std::begin(values_));
    }
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;
    constexpr fs_matrix_engine& operator=(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine& operator=(fs_matrix_engine const&) = default;

    template <class ET2>
    constexpr fs_matrix_engine& operator=(ET2 const& rhs)
    {
        if constexpr (is_fixed_size_engine_v<ET2>)
            static_assert(rows() == rhs.rows() && columns() == rhs.columns(), "Matrix dimensions must match.");

        using detail::times;
        for (auto [i, j] : times(rhs.rows()) * times(rhs.columns()))
            (*this)(i, j) = rhs(i, j);

        return *this;
    }

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return C; }
    constexpr size_type rows() const noexcept { return R; }
    constexpr size_tuple size() const noexcept { return {rows(), columns()}; }
    constexpr size_type column_capacity() const noexcept { return C; }
    constexpr size_type row_capacity() const noexcept { return R; }
    constexpr size_tuple capacity() const noexcept { return {R, C}; }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j)
    {
        return values_[i * column_capacity() + j];
    }

    constexpr const_reference operator()(size_type i, size_type j) const
    {
        return values_[i * column_capacity() + j];
    }

    //- Modifiers
    //
    constexpr void swap(fs_matrix_engine& rhs) noexcept { values_.swap(rhs.values_); }

    constexpr void swap_columns(size_type j1, size_type j2) noexcept
    {
        using detail::times;
        for (size_type i : times(rows()))
            std::swap((*this)(i, j1), (*this)(i, j2));
    }

    constexpr void swap_rows(size_type i1, size_type i2) noexcept
    {
        using detail::times;
        for (size_type j : times(columns()))
            std::swap((*this)(i1, j), (*this)(i2, j));
    }

  private:
    std::array<T, R * C> values_{};
};

} // end namespace

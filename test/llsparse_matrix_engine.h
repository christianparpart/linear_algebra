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

#include <linear_algebra>
#include <list>
#include <stdexcept>

/// Helper for assigning a value to a sparse matrix at given coordinates.
template <typename ET>
struct sparse_reference_wrapper {
    using size_type = typename ET::size_type;
    using element_type = typename ET::element_type;

    ET& engine;
    size_type const i;
    size_type const j;

    constexpr sparse_reference_wrapper(ET& _engine, size_type _i, size_type _j) noexcept : engine{_engine}, i{_i}, j{_j} {}

    constexpr sparse_reference_wrapper& operator=(element_type const& value) noexcept {
        engine.assign(i, j, value);
        return *this;
    }
    constexpr operator element_type const& () const noexcept { return engine(i, j); }
};

/// Naive(!) implementation of a sparse matrix via a list of a tuple of coordinates and their values.
template <typename T>
class llsparse_matrix_engine
{
  public:
    //- Types
    //
    using engine_category = LINEAR_ALGEBRA_NAMESPACE::resizable_matrix_engine_tag;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using size_tuple = std::tuple<size_type, size_type>;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using coord_type = size_tuple;
    using item_type = std::tuple<coord_type, value_type>;
    using pointer = element_type*;
    using const_pointer = element_type const*;

  private:
    value_type const zero_{};
    std::list<item_type> values_{};
    size_type columns_;
    size_type rows_;

  public:
    //using reference = element_type&;
    using reference = sparse_reference_wrapper<llsparse_matrix_engine<T>>;
    using const_reference = element_type const&;

  public:
    //- Construct/copy/destroy
    //
    ~llsparse_matrix_engine() noexcept = default;
    constexpr llsparse_matrix_engine() = default;

    template <typename U>
    constexpr llsparse_matrix_engine(size_type rows, size_type cols, std::initializer_list<U> _values)
    {
        resize(rows, cols);
        size_t i = 0;
        size_t j = 0;
        for (auto v : _values)
        {
            (*this)(i, j) = v;
            if (j == cols)
            {
                i++;
                j = 0;
            }
        }
    }
    constexpr llsparse_matrix_engine(llsparse_matrix_engine&&) noexcept = default;
    constexpr llsparse_matrix_engine(llsparse_matrix_engine const&) = default;
    constexpr llsparse_matrix_engine& operator=(llsparse_matrix_engine&&) noexcept = default;
    constexpr llsparse_matrix_engine& operator=(llsparse_matrix_engine const&) = default;

    template <class ET2>
    constexpr llsparse_matrix_engine& operator=(ET2 const& rhs)
    {
        resize(rhs.rows(), rhs.columns());
        using LINEAR_ALGEBRA_NAMESPACE::detail::times;
        for (auto [i, j] : times(rhs.rows()) * times(rhs.columns()))
            (*this)(i, j) = rhs(i, j);

        return *this;
    }

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return columns_; }
    constexpr size_type rows() const noexcept { return rows_; }
    constexpr size_tuple size() const noexcept { return {rows(), columns()}; }
    constexpr size_type column_capacity() const noexcept { return columns(); }
    constexpr size_type row_capacity() const noexcept { return rows(); }
    constexpr size_tuple capacity() const noexcept { return {rows(), columns()}; }

    /// Retrieves the number of actual elements stored.
    constexpr size_type elements() const noexcept { return values_.size(); }

    void reserve(size_type /*rowcap*/, size_type /*colcap*/)
    {
        // no-op
    }

    void resize(size_type rows, size_type cols)
    {
        if (rows > row_capacity() || cols > column_capacity())
            reserve(rows, cols);

        if (rows_ < rows || columns_ < cols)
        {
            // TODO: discard out-of-bounds values.
        }

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
    constexpr reference operator()(size_type i, size_type j)
    {
        if (i >= rows())
            throw std::out_of_range("Index out of bounds (i).");

        if (j >= columns())
            throw std::out_of_range("Index out of bounds (j).");

        return reference(*this, i, j);
    }

    constexpr void assign(size_type i, size_type j, value_type const& v)
    {
        if (i >= rows())
            throw std::out_of_range("Index out of bounds (i).");

        if (j >= columns())
            throw std::out_of_range("Index out of bounds (j).");

        if (v)
        {
            for (auto && [coord, value] : values_)
                if (std::get<0>(coord) == i && std::get<1>(coord) == j)
                {
                    value = v;
                    return;
                }

            values_.emplace_back(std::tuple{coord_type{i, j}, v});
        }
    }

    constexpr const_reference operator()(size_type i, size_type j) const
    {
        if (i >= rows())
            throw std::out_of_range("Index out of bounds (i).");
        if (j >= columns())
            throw std::out_of_range("Index out of bounds (j).");

        for (auto && [coord, value] : values_)
            if (std::get<0>(coord) == i && std::get<1>(coord) == j)
                return value;

        return zero_;
    }

    //- Modifiers
    //
    constexpr void swap(llsparse_matrix_engine& rhs) noexcept { values_.swap(rhs.values_); }

    constexpr void swap_columns(size_type j1, size_type j2) noexcept
    {
        using LINEAR_ALGEBRA_NAMESPACE::detail::times;
        for (size_type i : times(rows()))
            std::swap((*this)(i, j1), (*this)(i, j2));
    }

    constexpr void swap_rows(size_type i1, size_type i2) noexcept
    {
        using LINEAR_ALGEBRA_NAMESPACE::detail::times;
        for (size_type j : times(columns()))
            std::swap((*this)(i1, j), (*this)(i2, j));
    }
};

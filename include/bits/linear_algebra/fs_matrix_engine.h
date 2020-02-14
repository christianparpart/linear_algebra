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
class fs_matrix_engine {
  private:
    std::array<T, R * C> values_{};

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
        for (size_type i = 0; i < rhs.rows(); ++i)
            for (size_type j = 0; j < rhs.columns(); ++j)
                (*this)(i, j) = rhs(i, j);

        return *this;
    }

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return C; }
    constexpr size_type rows() const noexcept { return R; }
    constexpr size_tuple size() const noexcept { return {R, C}; }
    constexpr size_type column_capacity() const noexcept { return C; }
    constexpr size_type row_capacity() const noexcept { return R; }
    constexpr size_tuple capacity() const noexcept { return {R, C}; }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j)
    {
        assert(i < R);
        assert(j < C);
        return values_[i * C + j];
    }

    constexpr const_reference operator()(size_type i, size_type j) const
    {
        assert(i < R);
        assert(j < C);
        return values_[i * C + j];
    }

    //- Modifiers
    //
    constexpr void swap(fs_matrix_engine& rhs) noexcept { values_.swap(rhs.values_); }

    constexpr void swap_columns(size_type j1, size_type j2) noexcept
    {
        for (size_type i = 0; i < R; ++i)
            std::swap((*this)(i, j1), (*this)(i, j2));
    }

    constexpr void swap_rows(size_type i1, size_type i2) noexcept
    {
        for (size_type j = 0; j < C; ++j)
            std::swap((*this)(i1, j), (*this)(i2, j));
    }
};

} // end namespace

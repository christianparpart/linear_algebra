#pragma once

#include "base.h"

#include <cassert>

#include <array>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.3 | class fs_vector_engine<T, N>
template <class T, size_t N>
class fs_vector_engine {
  private:
    std::array<T, N> values_{};

  public:
    using engine_category = writable_vector_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using iterator = decltype(std::begin(values_));
    using const_iterator = decltype(std::cbegin(values_));

    //- Construct/copy/destroy
    //
    ~fs_vector_engine() noexcept = default;
    constexpr fs_vector_engine() = default;
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;
    template <class U>
    constexpr fs_vector_engine(std::initializer_list<U> list)
    {
        std::size_t i = 0;
        for (auto && v : list)
            values_[i++] = v;
    }
    constexpr fs_vector_engine& operator=(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine& operator=(fs_vector_engine const&) = default;

    //- Iterators
    //
    constexpr iterator begin() noexcept { return std::begin(values_); }
    constexpr iterator end() noexcept { return std::end(values_); }
    constexpr const_iterator begin() const noexcept { return std::begin(values_); }
    constexpr const_iterator end() const noexcept { return std::end(values_); }
    constexpr const_iterator cbegin() const noexcept { return std::cbegin(values_); }
    constexpr const_iterator cend() const noexcept { return std::cend(values_); }

    //- Capacity
    //
    static constexpr size_type capacity() noexcept { return N; }
    static constexpr size_type elements() noexcept { return N; }

    //- Element access
    //
    constexpr reference operator()(size_type i) { return values_[i]; }
    constexpr const_reference operator()(size_type i) const { return values_[i]; }

    //- Modifiers
    //
    constexpr void swap(fs_vector_engine& rhs) noexcept { std::swap(values_, rhs.values_); }
    constexpr void swap_elements(size_type i, size_type j) noexcept { std::swap(values_[i], values_[j]); }
};

} // end namespace

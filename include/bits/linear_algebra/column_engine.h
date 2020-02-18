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

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.5 | class column_engine<ET, VCT>
template <class ET, class VCT>
class column_engine {
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  private:
    template <typename ET2>
    struct _Iterator {
        using size_type = typename ET2::size_type;
        ET2& engine;
        size_type i;
        size_type j;
        _Iterator(ET2 const& _e, size_type _i, size_type _j) : engine{_e}, i{_i}, j{_j} {}
        constexpr decltype(auto) operator*() const noexcept { return engine(i, j); }
        constexpr decltype(auto) operator*() noexcept { return engine(i, j); }
        constexpr auto operator++() noexcept { ++j; return *this; }
        constexpr auto operator++(int) noexcept { ++j; return *this; }
        constexpr bool operator==(_Iterator const& other) const noexcept { return j == other.j && i == other.i && &engine == &other.engine; }
        constexpr bool operator!=(_Iterator const& other) const noexcept { return !(*this == other); }
    };

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = std::conditional_t<!is_readonly_engine_v<ET>, typename ET::pointer, typename ET::const_pointer>;
    using const_pointer = typename ET::const_pointer;
    using reference = std::conditional_t<!is_readonly_engine_v<ET>, typename ET::reference, typename ET::const_reference>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;

    using iterator = _Iterator<ET>; // Implementation-defined
    using const_iterator = _Iterator<std::add_const_t<ET>>; //- Implementation-defined

    //- Construct/copy/destroy
    //
    ~column_engine() noexcept = default;
    constexpr column_engine() noexcept = default;
    constexpr column_engine(column_engine&&) noexcept = default;
    constexpr column_engine(column_engine const&) noexcept = default;
    constexpr column_engine& operator=(column_engine&&) noexcept = default;
    constexpr column_engine& operator=(column_engine const&) noexcept = default;

    // EXT
    constexpr column_engine(ET& _engine, size_type _column) noexcept : engine_{&_engine}, column_{_column} {}

    //- Iterators
    //
    constexpr iterator begin() const noexcept { return iterator(*this, 0, column_); }
    constexpr iterator end() const noexcept { return iterator(*this, engine_->rows(), column_); }
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept { return engine_->row_capacity(); }
    constexpr size_type elements() const noexcept { return engine_->rows(); }

    //- Element access
    //
    constexpr reference operator()(size_type i) const { return (*engine_)(i, column_); }

    //- Modifiers
    //
    constexpr void swap(column_engine& rhs)
    {
        std::swap(column_, rhs.column_);
        std::swap(engine_, rhs.engine_);
    }

  private:
    ET* engine_ = nullptr;
    size_type column_ = 0;
};

} // end namespace

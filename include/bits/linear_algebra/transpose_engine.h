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

// 6.4.7 | class transpose_engine<ET, MCT>
template <class ET, class MCT>
class transpose_engine {
  private:
    ET* engine_{};

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = std::conditional_t<is_readonly_engine_v<ET>, typename ET::const_pointer, typename ET::pointer>;
    using const_pointer = typename ET::const_pointer;
    using reference = std::conditional_t<is_readonly_engine_v<ET>, typename ET::const_reference, typename ET::reference>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~transpose_engine() noexcept = default;
    constexpr transpose_engine() = default;
    constexpr transpose_engine(transpose_engine&&) noexcept = default;
    constexpr transpose_engine(transpose_engine const&) = default;
    constexpr transpose_engine& operator=(transpose_engine&&) noexcept = default;
    constexpr transpose_engine& operator=(transpose_engine const&) = default;

    // EXT
    constexpr explicit transpose_engine(ET* e) : engine_{e} {}

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return engine_->rows(); }
    constexpr size_type rows() const noexcept { return engine_->columns(); }
    constexpr size_tuple size() const noexcept{ return {rows(), columns()}; }
    constexpr size_type column_capacity() const noexcept { return columns(); }
    constexpr size_type row_capacity() const noexcept { return rows(); }
    constexpr size_tuple capacity() const noexcept { return {row_capacity(), column_capacity()}; }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const
    {
        return (*engine_)(j, i);
    }

    //- Modifiers
    //
    constexpr void swap(transpose_engine& rhs)
    {
        std::swap(engine_, rhs.engine_);
    }
};

} // end namespace

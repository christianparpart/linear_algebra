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

// 6.4.6
template <class ET, class MCT>
class matrix_view_engine<ET, MCT, transpose_view_tag> {
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
    using span_type = TODO; // implementation-defined ; (see note )
    using const_span_type = TODO; // implementation-defined ; (see note )

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine() = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine const&) = default;

    // EXT
    constexpr explicit matrix_view_engine(ET* e) : engine_{e} {}

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

    //- Data access
    //
    constexpr span_type span() const noexcept; // TODO

    //- Modifiers
    //
    constexpr void swap(matrix_view_engine& rhs)
    {
        std::swap(engine_, rhs.engine_);
    }
};

} // end namespace

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

// 6.4.8 | class submatrix_engine<ET, MCT>
template <class ET, class MCT>
class submatrix_engine {
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~submatrix_engine() noexcept = default;
    constexpr submatrix_engine();
    constexpr submatrix_engine(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine(submatrix_engine const&) = default;
    constexpr submatrix_engine& operator=(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine& operator=(submatrix_engine const&) = default;

    //- Capacity
    //
    constexpr size_type columns() const noexcept;
    constexpr size_type rows() const noexcept;
    constexpr size_tuple size() const noexcept;
    constexpr size_type column_capacity() const noexcept;
    constexpr size_type row_capacity() const noexcept;
    constexpr size_tuple capacity() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void swap(submatrix_engine& rhs);
};

} // end namespace

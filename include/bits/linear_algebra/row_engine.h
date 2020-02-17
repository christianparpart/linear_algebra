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

// 6.4.6 | class row_engine<ET, VCT>
template <class ET, class VCT>
class row_engine {
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using iterator = TODO; //- Implementation-defined
    using const_iterator = TODO; //- Implementation-defined

    //- Construct/copy/destroy
    //
    ~row_engine() noexcept = default;
    constexpr row_engine() noexcept;
    constexpr row_engine(row_engine&&) noexcept = default;
    constexpr row_engine(row_engine const&) noexcept = default;
    constexpr row_engine& operator=(row_engine&&) noexcept = default;
    constexpr row_engine& operator=(row_engine const&) noexcept = default;

    //- Iterators
    //
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept;
    constexpr size_type elements() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i) const;

    //- Modifiers
    //
    constexpr void swap(row_engine& rhs);
};

} // end namespace

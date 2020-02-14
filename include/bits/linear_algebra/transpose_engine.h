#pragma once

#include "base.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.7 | class transpose_engine<ET, MCT>
template <class ET, class MCT>
class transpose_engine {
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
    ~transpose_engine() noexcept = default;
    constexpr transpose_engine();
    constexpr transpose_engine(transpose_engine&&) noexcept = default;
    constexpr transpose_engine(transpose_engine const&) = default;
    constexpr transpose_engine& operator=(transpose_engine&&) noexcept = default;
    constexpr transpose_engine& operator=(transpose_engine const&) = default;

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
    constexpr void swap(transpose_engine& rhs);
};

} // end namespace

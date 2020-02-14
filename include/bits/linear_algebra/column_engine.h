#pragma once

#include "base.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.5 | class column_engine<ET, VCT>
template <class ET, class VCT>
class column_engine {
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
    ~column_engine() noexcept = default;
    constexpr column_engine() noexcept;
    constexpr column_engine(column_engine&&) noexcept = default;
    constexpr column_engine(column_engine const&) noexcept = default;
    constexpr column_engine& operator=(column_engine&&) noexcept = default;
    constexpr column_engine& operator=(column_engine const&) noexcept = default;

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
    constexpr void swap(column_engine& rhs);
};

} // end namespace

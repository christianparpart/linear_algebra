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

#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.1
template<class T, class AT>
class dr_vector_engine {
  private:
    std::vector<T, AT> elements_;

  public:
    //- Types
    //
    using engine_category = resizable_vector_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using allocator_type = AT;
    using pointer = typename std::allocator_traits<AT>::pointer;
    using const_pointer = typename std::allocator_traits<AT>::const_pointer;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    //- Construct/copy/destroy
    //
    ~dr_vector_engine() noexcept = default;
    dr_vector_engine() = default;
    dr_vector_engine(dr_vector_engine&&) noexcept = default;
    dr_vector_engine(dr_vector_engine const&) = default;
    template<class U>
    dr_vector_engine(std::initializer_list<U> list)
    {
        for (auto v : list)
            elements_.emplace_back(v);
    }
    dr_vector_engine(size_type elems)
    {
        resize(elems);
    }
    dr_vector_engine(size_type elems, size_type elem_cap)
    {
        reserve(elem_cap);
        resize(elems);
    }
    dr_vector_engine& operator =(dr_vector_engine&& rhs) noexcept = default;
    dr_vector_engine& operator =(dr_vector_engine const& rhs) = default;
    template<class ET2>
    dr_vector_engine& operator =(ET2 const& rhs)
    {
        elements_.clear();
        elements_.reserve(rhs.size());
        for (auto v : rhs)
            elements_.emplace_back(v);
        return *this;
    }

    //- Iterators
    //
    iterator begin() noexcept { return elements_.begin(); }
    iterator end() noexcept { return elements_.end(); }
    const_iterator begin() const noexcept { return elements_.begin(); }
    const_iterator end() const noexcept { return elements_.end(); }
    const_iterator cbegin() const noexcept { return elements_.cbegin(); }
    const_iterator cend() const noexcept { return elements_.cend(); }

    //- Capacity
    //
    size_type capacity() const noexcept { return elements_.capacity(); }
    size_type elements() const noexcept { return elements_.size(); }
    void reserve(size_type cap) { elements_.reserve(cap); }
    void resize(size_type elems) { elements_.resize(elems); }
    void resize(size_type elems, size_type cap)
    {
        elements_.reserve(cap);
        elements_.resize(elems);
    }

    //- Element access
    //
    reference operator ()(size_type i) { return elements_[i]; }
    const_reference operator ()(size_type i) const { return elements_[i]; }

    //- Modifiers
    //
    void swap(dr_vector_engine& rhs) noexcept { std::swap(elements_, rhs.elements_); }
    void swap_elements(size_type i, size_type j) noexcept { std::swap(elements_[i], elements_[j]); }
};

} // end namespace

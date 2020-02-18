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

#include <iterator>

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.5.1 | class vector<ET, OT>
template <class ET, class OT>
class vector {
  private:
    ET engine_{};

  public:
    //- Types
    //
    using engine_type = ET;
    using element_type = typename engine_type::element_type;
    using value_type = typename engine_type::value_type;
    using difference_type = typename engine_type::difference_type;
    using size_type = typename engine_type::size_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using iterator = typename engine_type::iterator; //- Implementation-defined
    using const_iterator = typename engine_type::const_iterator; //- Implementation-defined
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using transpose_type = vector&;
    using const_transpose_type = vector const&;
    using hermitian_type = std::conditional_t<detail::is_complex_v<element_type>, vector, transpose_type>;
    using const_hermitian_type = std::conditional_t<detail::is_complex_v<element_type>, vector, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~vector() = default;
    constexpr vector() = default;
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;
    template <class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src) //: engine_(src.engine()) {}
    {
        for (auto i : detail::times(src.size()))
            engine_(i) = src(i);
    }
    template <class U>
    constexpr vector(std::initializer_list<U> list) : engine_(list) {}
    constexpr vector(size_type elems) { resize(elems); }
    constexpr vector(size_type elems, size_type elemcap) { resize(elems, elemcap); }
    constexpr explicit vector(ET&& _engine) : engine_{std::forward<ET>(_engine)} {} // EXT
    constexpr vector& operator=(vector&&) noexcept = default;
    constexpr vector& operator=(vector const&) = default;
    template <class ET2, class OT2>
    constexpr vector& operator=(vector<ET2, OT2> const& rhs)
    {
        std::size_t i = 0;
        for (auto && v : rhs.engine())
            engine_(i++) = v;

        return *this;
    }

    //- Iterators
    //
    constexpr iterator begin() noexcept { return engine_.begin(); }
    constexpr const_iterator begin() const noexcept { return engine_.begin(); }
    constexpr iterator end() noexcept { return engine_.end(); }
    constexpr const_iterator end() const noexcept { return engine_.end(); }
    // TODO constexpr reverse_iterator rbegin() noexcept;
    // TODO constexpr const_reverse_iterator rbegin() const noexcept;
    // TODO constexpr reverse_iterator rend() noexcept;
    // TODO constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_iterator cbegin() const noexcept { return engine_.cbegin(); }
    constexpr const_iterator cend() const noexcept { return engine_.cend(); }
    // TODO constexpr const_reverse_iterator crbegin() const noexcept;
    // TODO constexpr const_reverse_iterator crend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept { return engine_.capacity(); }
    constexpr size_type elements() const noexcept { return engine_.elements(); }
    constexpr size_type size() const noexcept { return engine_.elements(); }
    constexpr void reserve([[maybe_unused]] size_type elemcap)
    {
        // static_assert(std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.reserve(elemcap);
    }
    constexpr void resize([[maybe_unused]] size_type elems)
    {
        reserve(elems);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.resize(elems);
    }
    constexpr void resize([[maybe_unused]] size_type elems, [[maybe_unused]] size_type elemcap)
    {
        //static_assert(std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.resize(elems, elemcap);
    }

    //- Element access
    //
    constexpr reference operator[](size_type i) { return engine_(i); }
    constexpr reference operator()(size_type i) { return engine_(i); }
    constexpr const_reference operator[](size_type i) const { return engine_(i); }
    constexpr const_reference operator()(size_type i) const { return engine_(i); }
    constexpr transpose_type t() { return *this; }
    constexpr const_transpose_type t() const { return *this; }
    constexpr hermitian_type h()
    {
        auto ht = hermitian_type(size());
        size_t i = 0;
        for (auto && v : *this)
        {
            ht(i) = (*this)(i);
            ++i;
        }
        return ht;
    }
    constexpr const_hermitian_type h() const
    {
        auto ht = hermitian_type(size());
        size_t i = 0;
        for (auto && v : *this)
        {
            ht(i) = (*this)(i);
            ++i;
        }
        return ht;
    }

    //- Data access
    //
    constexpr engine_type& engine() noexcept { return engine_; }
    constexpr engine_type const& engine() const noexcept { return engine_; }

    //- Modifiers
    //
    constexpr void swap(vector& rhs) noexcept;
    constexpr void swap_elements(size_type i, size_type j) noexcept;
};

} // end namespace

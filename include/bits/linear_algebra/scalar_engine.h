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
#include <type_traits>

namespace LINEAR_ALGEBRA_NAMESPACE {

/// Non-owning dummy engine for scalar operands.
template <typename T>
struct scalar_engine {
    using engine_category = scalar_engine_tag;

    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = TODO; //- See below
    using const_pointer = std::add_const_t<std::add_pointer_t<value_type>>;
    using reference = std::add_lvalue_reference<value_type>;
    using const_reference = std::add_const_t<reference>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
};

} // end namespace

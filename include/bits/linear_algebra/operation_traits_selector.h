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

// ---------------------------------------------------------------------------
// 6.6.2 | matrix_operation_traits_selector
// Primary template and expected specializations.

template <class T1, class T2>
struct matrix_operation_traits_selector;

template <class T1>
struct matrix_operation_traits_selector<T1, T1>
{
    using traits_type = T1;
};

template <class T1>
struct matrix_operation_traits_selector<T1, matrix_operation_traits>
{
    using traits_type = T1;
};

template <class T1>
struct matrix_operation_traits_selector<matrix_operation_traits, T1>
{
    using traits_type = T1;
};

template <>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

//- Convenience alias.
template <class T1, class T2>
using matrix_operation_traits_selector_t = typename matrix_operation_traits_selector<T1, T2>::traits_type;

} // end namespace

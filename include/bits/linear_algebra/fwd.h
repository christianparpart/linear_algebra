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

template <typename ET, typename OT> class vector;
template <typename ET, typename OT> class matrix;

template <typename T, size_t N> class fs_vector_engine;
template <typename T, size_t R, size_t C> class fs_matrix_engine;

template <typename T> struct scalar_engine;
template <typename ET, typename OT> class row_engine;
template <typename ET, typename OT> class column_engine;
template <typename ET, typename OT> class transpose_engine;
template <typename ET, typename OT> class submatrix_engine;

} // end namespace

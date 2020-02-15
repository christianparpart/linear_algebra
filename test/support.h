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

#include <linear_algebra>
#include <ostream>

template <typename T, size_t N> using vec = LINEAR_ALGEBRA_NAMESPACE::fs_vector<T, N>;
template <size_t N> using ivec = vec<int, N>;

template <typename T, size_t R, size_t C> using mat = LINEAR_ALGEBRA_NAMESPACE::fs_matrix<T, R, C>;
template <size_t R, size_t C> using imat = mat<int, R, C>;


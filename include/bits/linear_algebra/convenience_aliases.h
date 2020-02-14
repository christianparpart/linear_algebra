#pragma once

#include "base.h"
#include "fs_vector_engine.h"
#include "fs_matrix_engine.h"
#include "operation_traits.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

template <class T, int32_t N>
using fs_vector = vector<fs_vector_engine<T, N>, matrix_operation_traits>;

template <class T, int32_t R, int32_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>, matrix_operation_traits>;

} // end namespace

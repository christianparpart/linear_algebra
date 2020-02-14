#pragma once

#include "base.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

template <typename ET, typename OT> class vector;
template <typename ET, typename OT> class matrix;

template <typename T, size_t N> class fs_vector_engine;
template <typename T, size_t R, size_t C> class fs_matrix_engine;

template <typename T> class scalar_engine;
template <typename ET, typename OT> class row_engine;
template <typename ET, typename OT> class column_engine;
template <typename ET, typename OT> class transpose_engine;
template <typename ET, typename OT> class submatrix_engine;

} // end namespace

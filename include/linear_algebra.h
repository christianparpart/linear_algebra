#pragma once
// vim:syntax=cpp:ft=cpp:et

#include "bits/linear_algebra/base.h"

// operation traits
#include "bits/linear_algebra/operation_traits.h"

// owning engines
#include "bits/linear_algebra/fs_matrix_engine.h"
#include "bits/linear_algebra/fs_vector_engine.h"

// non-owning engines
#include "bits/linear_algebra/scalar_engine.h"
#include "bits/linear_algebra/column_engine.h"
#include "bits/linear_algebra/row_engine.h"
#include "bits/linear_algebra/submatrix_engine.h"
#include "bits/linear_algebra/transpose_engine.h"

// math objects
#include "bits/linear_algebra/vector.h"
#include "bits/linear_algebra/matrix.h"

#include "bits/linear_algebra/arithmetic_operators.h"
#include "bits/linear_algebra/convenience_aliases.h"

// stuff that wasn't mentioned in the paper
#include "bits/linear_algebra/ext.h"


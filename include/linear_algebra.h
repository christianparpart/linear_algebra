#pragma once
// vim:syntax=cpp:ft=cpp:et

/**
 * - Parameter names T, T1, and T2 represent element types.
 * — Parameter names ET, ET1, and ET2 represent engine types.
 * — Parameter names OT, OT1, and OT2 represent operation traits types.
 * — Parameter names OP, OP1, and OP2 represent the operand types deduced by an arithmetic operator.
 * — Parameter names AT, AT1, and AT2 represent allocator types.
 * — Parameter names N, N1, and N2 represent the number of elements in a fixed-size vector.
 * — Parameter names C, C1, and C2 represent the number of columns in a fixed-size matrix.
 * — Parameter names R, R1, and R2 represent the number of rows in a fixed-size matrix.
 * — Parameter name VCT represents a vector engine’s category tag type.
 * — Parameter name MCT represents a matrix engine’s category tag type.
 */

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


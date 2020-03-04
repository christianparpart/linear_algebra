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

#include "bits/linear_algebra/base.h"

// operation traits
#include "bits/linear_algebra/operation_traits.h"

// owning engines
#include "bits/linear_algebra/fs_vector_engine.h"
#include "bits/linear_algebra/fs_matrix_engine.h"
#include "bits/linear_algebra/dr_vector_engine.h"
#include "bits/linear_algebra/dr_matrix_engine.h"

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
#include "bits/linear_algebra/ext_permutation.h"
#include "bits/linear_algebra/ext_det.h"


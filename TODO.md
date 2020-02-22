## PROPOSAL

* [ ] `matrix.t()` (tranpose matrix)
* [ ] `matrix.h()` (hermitian matrix)
* [ ] `vector` reverse iterators
* [ ] `vector` free function `begin()`/`end()`
* [ ] ensure proper engine promotion (fs op dr -> dr; dr op fs -> dr)
* [ ] ensure proper trait promotion (std op custom -> custom, and vice versa)

## EXTS

* [ ] elementary matrix construction
* [ ] `sparse_matrix_engine<OT>`
* [x] `matrix(Initializer init)` for lambda initializer (i, j) -> T
* [ ] `vector(Initializer init)` for lambda initializer (i) -> T
* [ ] `one<T>`, `zero<T>` and some kind of `numeric_traits<T>`
* [ ] `trace(A)`
* [ ] `identity()`
* [ ] `cofactor(A, i, j)`
* [ ] `adjugate(A)`
* [x] `det(A)`, fixed-size matrix, with optimizations for 1x1, 2x2, 3x3
* [x] `det(A)`, dynamic-size matrix, with optimizations for 1x1, 2x2, 3x3
* [x] `det(A)`, Laplace for N >= 4
* [ ] `det(A)`, Leibnitz algorithm
* [ ] `inverse(A)`
* [ ] `solve(A)`
* [ ] `solve_traced(A)`
* [ ] `kronecker_delta(i, j)`

## Documentation

* [ ] `matrix<ET, OT>`
* [ ] `vector<ET, OT>`


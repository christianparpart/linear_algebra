## PROPOSAL

* [ ] ensure proper engine promotion (fs op dr -> dr; dr op fs -> dr)
* [ ] ensure proper trait promotion (std op custom -> custom, and vice versa)
* [ ] `matrix.h()` (hermitian matrix)
* [ ] `vector.h()` (hermitian matrix)

## EXTS

* [ ] elementary matrix construction
* [ ] `sparse_matrix_engine<OT>`
* [x] `matrix(Initializer init)` for lambda initializer (i, j) -> T
* [ ] `vector(Initializer init)` for lambda initializer (i) -> T
* [ ] `one<T>`, `zero<T>` and some kind of `numeric_traits<T>`
* [x] `trace(A)`
* [x] `kronecker_delta(i, j)`
* [x] `identity()`
* [x] `cofactor(A, i, j)`
* [x] `adjugate(A)`
* [x] `det(A)`, fixed-size matrix, with optimizations for 1x1, 2x2, 3x3
* [x] `det(A)`, dynamic-size matrix, with optimizations for 1x1, 2x2, 3x3
* [x] `det(A)`, Laplace for N >= 4
* [ ] `det(A)`, Leibnitz algorithm
* [x] `inverse(A)`
* [ ] `solve(A)`
* [ ] `solve_traced(A)`
* [ ] function for computing eigen values
* [ ] function to compute eigen vectors

## Documentation

* [ ] `matrix<ET, OT>`
* [ ] `vector<ET, OT>`


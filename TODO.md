## PROJECT

* [ ] GitHub Actions CI: GCC 9 / Ubuntu latest
* [ ] GitHub Actions CI: Clang 9 / Ubuntu latest
* [ ] GitHub Actions CI: Clang / OSX
* [ ] GitHub Actions CI: MSVC / Windows
* [ ] tests: custom operation_traits

## PROPOSAL

* [ ] `dr_vector_engine`
* [ ] `dr_matrix_engine`
* [ ] ensure proper engine promotion (fs op dr -> dr; dr op fs -> dr)
* [ ] ensure proper trait promotion (std op custom -> custom, and vice versa)

## EXTS

* [ ] elementary matrix construction
* [ ] matrix construction by lambda
* [ ] `sparse_matrix_engine<OT>`
* [ ] `matrix(Initializer init)` for lambda initializer (i, j) -> T
* [ ] `vector(Initializer init)` for lambda initializer (i) -> T
* [ ] `one<T>`, `zero<T>` and some kind of `numeric_traits<T>`
* [ ] `trace(A)`
* [ ] `cofactor(i, j, A)`
* [ ] `adjugate(A)`
* [ ] `det(A)`, with optimizations for 1x1, 2x2, 3x3 (fs & dr), Laplace for N >= 4
* [ ] `inverse(A)`
* [ ] `solve(A)`
* [ ] `solve_traced(A)`
* [ ] `kronecker_delta(i, j)`

## Documentation

* [ ] `matrix<ET, OT>`
* [ ] `vector<ET, OT>`



## ERATA
* `matrix<ET, OT>::matrix(initializer_list<U>)`
	is missing `template<typename U>`
* 6.8.2, text for `matrix_addition_engine_t` is wrong (wrongly referring to `OT::element_addition_traits`)
* sometims `ptrdiff_t` sometimes `std::ptrdiff_t`
* `matrix_operation_traits::negation_traits<OTR, OP1>`

## Feedback
- tags: readable/writable/resizable are definitely confusing to what they actually mean
* Why `template <class>` over `template <typename>` when it is clear that it is a type?
* What about the "See below"'s?
* No words on `scalar_engine` except that it's a dummy scalar engine? ;-()
* `detail::engine_add_type_t` is mentioned without any comments.
* `detail::element_tag<T>` is confusing (and not documented).
* A little too often the paper says "implementation defined" without any words on what the
	non-trivial types are meant to do (I'm clearly not referring to `iterator`/`const_iterator` here).
* Matrix comparison missing (e.g. operator==)
* `submatrix(r, c)` should be a convenience`method for `submatrix(r, r, c, c)`
* why not prefer free functions over member functions (for matrix and vector) for operations such as
	transpose, and submatrix. This would allow making it more streamlined
	with potential operation extensions, such as a user wants to also provide a determinant function
	without breaking API design (free function vs member functions)
* Missing constexpr on operator+ and related. This is a huge must (for me personally).
* Deduction guides for square matrix and vector would be nice.
* `vector::transpose_type` is `vector&`? shouldn't it be a matrix of row size 1? so that
    myvec.t() * myvec == square matrix of size myvec.size()?
* matrix ctor with `initializer(i, j) -> T` lambda
* vector ctor with `initializer(i) -> T` lambda
* `matrix__...__engine_traits::element_type` should not be there. (can be reached via `...::engine_type::element_type`)
    * actually, only `matrix_..._engine_t<>` should be exposed.
* From a Math point of view, you name a matrix like M_{m,n}(F) so it would be natural
  to say `matrix<M, N, F>` instead of `matrix<F, M, N>`. Why the other way around?
* Why no expression templates?

## THOUGHTS
* What about bigint/bigrational/symexpr numeric types? (seperate paper/proposal)

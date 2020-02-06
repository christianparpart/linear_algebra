#pragma once
#include <type_traits>
#include <iterator>
#include <initializer_list>

namespace math {
// ----------------------------------------------------------------------------------------------
// 6.5.1

template<class ET, class OT>
class vector {
  public:
    //- Types
    //
    using engine_type = ET;
    using element_type = typename engine_type::element_type;
    using value_type = typename engine_type::value_type;
    using difference_type = typename engine_type::difference_type;
    using size_type = typename engine_type::size_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using iterator = ...; //- Implementation-defined
    using const_iterator = ...; //- Implementation-defined
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using transpose_type = vector&;
    using const_transpose_type = vector const&;
    using hermitian_type = conditional_t<IsComplex, vector, transpose_type>;
    using const_hermitian_type = conditional_t<IsComplex, vector, const_transpose_type>;
    //- Construct/copy/destroy
    //
    ~vector() = default;
    constexpr vector();
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src);
    template<class U>
    constexpr vector(std::initializer_list<U> list);
    constexpr vector(size_type elems);
    constexpr vector(size_type elems, size_type elemcap);
    constexpr vector& operator =(vector&&) noexcept = default;
    constexpr vector& operator =(vector const&) = default;
    template<class ET2, class OT2>
    constexpr vector& operator =(vector<ET2, OT2> const& rhs);
    //- Iterators
    //
    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;
    //- Capacity
    //
    constexpr size_type capacity() const noexcept;
    constexpr size_type elements() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr void reserve(size_type elemcap);
    constexpr void resize(size_type elems);
    constexpr void resize(size_type elems, size_type elemcap);
    //- Element access
    //
    constexpr reference operator [](size_type i);
    constexpr reference operator ()(size_type i);
    constexpr const_reference operator [](size_type i) const;
    constexpr const_reference operator ()(size_type i) const;
    constexpr transpose_type t();
    constexpr const_transpose_type t() const;
    constexpr hermitian_type h();
    constexpr const_hermitian_type h() const;
    //- Data access
    //
    constexpr engine_type& engine() noexcept;
    constexpr engine_type const& engine() const noexcept;
    //- Modifiers
    //
    constexpr void swap(vector& rhs) noexcept;
    constexpr void swap_elements(size_type i, size_type j) noexcept;
};

// ----------------------------------------------------------------------------------------------
// 6.5.2

template<class ET, class OT>
class matrix {
  public:
    //- Types
    //
    using engine_type = ET;
    using element_type = typename engine_type::element_type;
    using value_type = typename engine_type::value_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;
    using difference_type = typename engine_type::difference_type;
    using size_type = typename engine_type::size_type;
    using size_tuple = typename engine_type::size_tuple;
    using column_type = vector<column_engine<ET, /*see below*/ >, OT>;
    using const_column_type = vector<column_engine<ET, readable_vector_engine_tag>, OT>;
    using row_type = vector<row_engine<ET, /*see below*/ >, OT>;
    using const_row_type = vector<row_engine<ET, readable_vector_engine_tag>, OT>;
    using submatrix_type = matrix<submatrix_engine<ET, /*see below*/ >, OT>;
    using const_submatrix_type = matrix<submatrix_engine<ET, readable_matrix_engine_tag>, OT>;
    using transpose_type = matrix<transpose_engine<ET, /*see below*/ >, OT>;
    using const_transpose_type = matrix<transpose_engine<ET, readable_matrix_engine_tag>, OT>;
    using hermitian_type = conditional_t<IsComplex, matrix, transpose_type>;
    using const_hermitian_type = conditional_t<IsComplex, matrix, const_transpose_type>;
    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;
    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;
    constexpr matrix(initializer_list<U> list);
    template<class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src);
    constexpr matrix(size_tuple size);
    constexpr matrix(size_type rows, size_type cols);
    constexpr matrix(size_tuple size, size_tuple cap);
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    constexpr matrix& operator =(matrix&&) noexcept = default;
    constexpr matrix& operator =(matrix const&) = default;
    template<class ET2, class OT2>
    constexpr matrix& operator =(matrix<ET2, OT2> const& rhs);
    //- Capacity
    //
    constexpr size_type columns() const noexcept;
    constexpr size_type rows() const noexcept;
    constexpr size_tuple size() const noexcept;
    constexpr size_type column_capacity() const noexcept;
    constexpr size_type row_capacity() const noexcept;
    constexpr size_tuple capacity() const noexcept;
    constexpr void reserve(size_tuple cap);
    constexpr void reserve(size_type rowcap, size_type colcap);
    constexpr void resize(size_tuple size);
    constexpr void resize(size_type rows, size_type cols);
    constexpr void resize(size_tuple size, size_tuple cap);
    constexpr void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    //- Element access
    //
    constexpr reference operator ()(size_type i, size_type j);
    constexpr const_reference operator ()(size_type i, size_type j) const;
    //- Columns, rows, submatrices, transposes, and the Hermitian
    //
    constexpr column_type column(size_type j) noexcept;
    constexpr const_column_type column(size_type j) const noexcept;
    constexpr row_type row(size_type i) noexcept;
    constexpr const_row_type row(size_type i) const noexcept;
    constexpr submatrix_type submatrix(size_type ri, size_type rn,
    size_type ci, size_type cn) noexcept;
    constexpr const_submatrix_type submatrix(size_type ri, size_type rn,
    size_type ci, size_type cn) const noexcept;
    constexpr transpose_type t() noexcept;
    constexpr const_transpose_type t() const noexcept;
    constexpr hermitian_type h();
    constexpr const_hermitian_type h() const;
    //- Data access
    //
    constexpr engine_type& engine() noexcept;
    constexpr engine_type const& engine() const noexcept;
    //- Modifiers
    //
    constexpr void swap(matrix& rhs) noexcept;
    constexpr void swap_columns(size_type i, size_type j) noexcept;
    constexpr void swap_rows(size_type i, size_type j) noexcept;
};

} // end namespace

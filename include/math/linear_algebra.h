#pragma once
// vim:syntax=cpp:ft=cpp:et

#include <cassert>
#include <cstdint>
#include <algorithm>
#include <complex>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>

#define LINEAR_ALGEBRA_NAMESPACE math

using TODO = void*; // XXX well, yeah ...

namespace LINEAR_ALGEBRA_NAMESPACE {

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

// {{{ util
namespace util {
    template <typename T> struct is_complex : public std::false_type {};
    template <typename T> struct is_complex<std::complex<const T>> : public is_complex<T> {};
    template <typename T> struct is_complex<std::complex<const volatile T>> : public is_complex<T> {};
    template <typename T> struct is_complex<std::complex<volatile T>> : public is_complex<T> {};
    template <typename T> struct is_complex<std::complex<T>> : public std::true_type {};
    template <typename T> constexpr inline bool is_complex_v = is_complex<T>::value;
}
// }}}

//- Tags that describe engines and their capabilities.
//
struct scalar_engine_tag {};
struct readable_vector_engine_tag {};   // i.e. vector read-only
struct writable_vector_engine_tag {};   // i.e. vector read-write fixed-size
struct resizable_vector_engine_tag {};  // i.e. vector read-write resizable
struct readable_matrix_engine_tag {};   // i.e. matrix read-only
struct writable_matrix_engine_tag {};   // i.e. matrix read-write fixed-size
struct resizable_matrix_engine_tag {};  // i.e. matrix read-write resizable

template <class T> constexpr inline bool is_matrix_element_v =
    std::is_arithmetic_v<T> ||
    util::is_complex_v<T>;

template <class T> constexpr inline bool is_matrix_engine_v =
    std::is_same_v<typename T::engine_category, readable_matrix_engine_tag> ||
    std::is_same_v<typename T::engine_category, writable_matrix_engine_tag> ||
    std::is_same_v<typename T::engine_category, resizable_matrix_engine_tag>;

template <class VCT> constexpr inline bool is_vector_engine_tag =
    std::is_same_v<VCT, readable_vector_engine_tag> ||
    std::is_same_v<VCT, writable_vector_engine_tag> ||
    std::is_same_v<VCT, resizable_vector_engine_tag>;

// {{{ 6.4.3 | class fs_vector_engine<T, N>
template <class T, size_t N>
class fs_vector_engine {
  private:
    std::array<T, N> values_{};

  public:
    using engine_category = writable_vector_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using iterator = decltype(std::begin(values_));
    using const_iterator = decltype(std::cbegin(values_));

    //- Construct/copy/destroy
    //
    ~fs_vector_engine() noexcept = default;
    constexpr fs_vector_engine() = default;
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;
    template <class U>
    constexpr fs_vector_engine(std::initializer_list<U> list)
    {
        std::size_t i = 0;
        for (auto && v : list)
            values_[i++] = v;
    }
    constexpr fs_vector_engine& operator=(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine& operator=(fs_vector_engine const&) = default;

    //- Iterators
    //
    constexpr iterator begin() noexcept { return std::begin(values_); }
    constexpr iterator end() noexcept { return std::end(values_); }
    constexpr const_iterator begin() const noexcept { return std::begin(values_); }
    constexpr const_iterator end() const noexcept { return std::end(values_); }
    constexpr const_iterator cbegin() const noexcept { return std::cbegin(values_); }
    constexpr const_iterator cend() const noexcept { return std::cend(values_); }

    //- Capacity
    //
    static constexpr size_type capacity() noexcept { return N; }
    static constexpr size_type elements() noexcept { return N; }

    //- Element access
    //
    constexpr reference operator()(size_type i) { return values_[i]; }
    constexpr const_reference operator()(size_type i) const { return values_[i]; }

    //- Modifiers
    //
    constexpr void swap(fs_vector_engine& rhs) noexcept { std::swap(values_, rhs.values_); }
    constexpr void swap_elements(size_type i, size_type j) noexcept { std::swap(values_[i], values_[j]); }
};
// }}}
// {{{ 6.4.4 | class fs_matrix_engine<T, R, C>
template <class T, size_t R, size_t C>
class fs_matrix_engine {
  private:
    std::array<T, R * C> values_{};

  public:
    //- Types
    //
    using engine_category = writable_matrix_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using size_tuple = std::tuple<size_type, size_type>;

    //- Construct/copy/destroy
    //
    ~fs_matrix_engine() noexcept = default;
    constexpr fs_matrix_engine() = default;

    template <typename U>
    constexpr fs_matrix_engine(std::initializer_list<U> _values)
    {
        size_t i = 0;
        for (auto v : _values)
            values_[i++] = v;
        //TODO(constexpr) std::copy(std::begin(_values), std::end(_values), std::begin(values_));
    }
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;
    constexpr fs_matrix_engine& operator=(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine& operator=(fs_matrix_engine const&) = default;

    template <class ET2>
    constexpr fs_matrix_engine& operator=(ET2 const& rhs)
    {
        for (size_type i = 0; i < rhs.rows(); ++i)
            for (size_type j = 0; j < rhs.columns(); ++j)
                (*this)(i, j) = rhs(i, j);

        return *this;
    }

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return C; }
    constexpr size_type rows() const noexcept { return R; }
    constexpr size_tuple size() const noexcept { return {R, C}; }
    constexpr size_type column_capacity() const noexcept { return C; }
    constexpr size_type row_capacity() const noexcept { return R; }
    constexpr size_tuple capacity() const noexcept { return {R, C}; }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j)
    {
        assert(i < R);
        assert(j < C);
        return values_[i * C + j];
    }

    constexpr const_reference operator()(size_type i, size_type j) const
    {
        assert(i < R);
        assert(j < C);
        return values_[i * C + j];
    }

    //- Modifiers
    //
    constexpr void swap(fs_matrix_engine& rhs) noexcept { values_.swap(rhs.values_); }

    constexpr void swap_columns(size_type j1, size_type j2) noexcept
    {
        for (size_type i = 0; i < R; ++i)
            std::swap((*this)(i, j1), (*this)(i, j2));
    }

    constexpr void swap_rows(size_type i1, size_type i2) noexcept
    {
        for (size_type j = 0; j < C; ++j)
            std::swap((*this)(i1, j), (*this)(i2, j));
    }
};
// }}}
// {{{ 6.4.5 | class column_engine<ET, VCT>
template <class ET, class VCT>
class column_engine {
  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using iterator = TODO; //- Implementation-defined
    using const_iterator = TODO; //- Implementation-defined

    //- Construct/copy/destroy
    //
    ~column_engine() noexcept = default;
    constexpr column_engine() noexcept;
    constexpr column_engine(column_engine&&) noexcept = default;
    constexpr column_engine(column_engine const&) noexcept = default;
    constexpr column_engine& operator=(column_engine&&) noexcept = default;
    constexpr column_engine& operator=(column_engine const&) noexcept = default;

    //- Iterators
    //
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept;
    constexpr size_type elements() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i) const;

    //- Modifiers
    //
    constexpr void swap(column_engine& rhs);
};
// }}}
// {{{ 6.4.6 | class row_engine<ET, VCT>
template <class ET, class VCT>
class row_engine {
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_vector_engine_tag<VCT>);

  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using iterator = TODO; //- Implementation-defined
    using const_iterator = TODO; //- Implementation-defined

    //- Construct/copy/destroy
    //
    ~row_engine() noexcept = default;
    constexpr row_engine() noexcept;
    constexpr row_engine(row_engine&&) noexcept = default;
    constexpr row_engine(row_engine const&) noexcept = default;
    constexpr row_engine& operator=(row_engine&&) noexcept = default;
    constexpr row_engine& operator=(row_engine const&) noexcept = default;

    //- Iterators
    //
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept;
    constexpr size_type elements() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i) const;

    //- Modifiers
    //
    constexpr void swap(row_engine& rhs);
};
// }}}
// {{{ 6.4.7 | class transpose_engine<ET, MCT>
template <class ET, class MCT>
class transpose_engine {
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~transpose_engine() noexcept = default;
    constexpr transpose_engine();
    constexpr transpose_engine(transpose_engine&&) noexcept = default;
    constexpr transpose_engine(transpose_engine const&) = default;
    constexpr transpose_engine& operator=(transpose_engine&&) noexcept = default;
    constexpr transpose_engine& operator=(transpose_engine const&) = default;

    //- Capacity
    //
    constexpr size_type columns() const noexcept;
    constexpr size_type rows() const noexcept;
    constexpr size_tuple size() const noexcept;
    constexpr size_type column_capacity() const noexcept;
    constexpr size_type row_capacity() const noexcept;
    constexpr size_tuple capacity() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void swap(transpose_engine& rhs);
};
// }}}
// {{{ 6.4.8 | class submatrix_engine<ET, MCT>
template <class ET, class MCT>
class submatrix_engine {
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = TODO; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = TODO; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;

    //- Construct/copy/destroy
    //
    ~submatrix_engine() noexcept = default;
    constexpr submatrix_engine();
    constexpr submatrix_engine(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine(submatrix_engine const&) = default;
    constexpr submatrix_engine& operator=(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine& operator=(submatrix_engine const&) = default;

    //- Capacity
    //
    constexpr size_type columns() const noexcept;
    constexpr size_type rows() const noexcept;
    constexpr size_tuple size() const noexcept;
    constexpr size_type column_capacity() const noexcept;
    constexpr size_type row_capacity() const noexcept;
    constexpr size_tuple capacity() const noexcept;

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void swap(submatrix_engine& rhs);
};
// }}}
// {{{ scalar_engine<T>

/// Non-owning dummy engine for scalar operands.
template <typename T>
struct scalar_engine {
    using engine_category = scalar_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using pointer = TODO; //- See below
    using const_pointer = std::add_const_t<std::add_pointer_t<value_type>>;
    using reference = std::add_lvalue_reference<value_type>;
    using const_reference = std::add_const_t<reference>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
};
// }}}
// {{{ 6.5.1 | class vector<ET, OT>
template <class ET, class OT>
class vector {
  private:
    ET engine_{};

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
    using iterator = typename engine_type::iterator; //- Implementation-defined
    using const_iterator = typename engine_type::const_iterator; //- Implementation-defined
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using transpose_type = vector&;
    using const_transpose_type = vector const&;
    using hermitian_type = std::conditional_t<util::is_complex_v<element_type>, vector, transpose_type>;
    using const_hermitian_type = std::conditional_t<util::is_complex_v<element_type>, vector, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~vector() = default;
    constexpr vector() = default;
    constexpr vector(vector&&) noexcept = default;
    constexpr vector(vector const&) = default;
    template <class ET2, class OT2>
    constexpr vector(vector<ET2, OT2> const& src) : engine_{src.engine_} {}
    template <class U>
    constexpr vector(std::initializer_list<U> list)
    {
        std::size_t i = 0;
        for (auto && v : list)
            engine_(i++) = v;
    }
    constexpr vector(size_type elems) { resize(elems); }
    constexpr vector(size_type elems, size_type elemcap) { resize(elems, elemcap); }
    constexpr vector& operator=(vector&&) noexcept = default;
    constexpr vector& operator=(vector const&) = default;
    template <class ET2, class OT2>
    constexpr vector& operator=(vector<ET2, OT2> const& rhs)
    {
        std::size_t i = 0;
        for (auto && v : rhs.engine())
            engine_(i++) = v;

        return *this;
    }

    //- Iterators
    //
    constexpr iterator begin() noexcept { return engine_.begin(); }
    constexpr const_iterator begin() const noexcept { return engine_.begin(); }
    constexpr iterator end() noexcept { return engine_.end(); }
    constexpr const_iterator end() const noexcept { return engine_.end(); }
    // TODO constexpr reverse_iterator rbegin() noexcept;
    // TODO constexpr const_reverse_iterator rbegin() const noexcept;
    // TODO constexpr reverse_iterator rend() noexcept;
    // TODO constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_iterator cbegin() const noexcept { return engine_.cbegin(); }
    constexpr const_iterator cend() const noexcept { return engine_.cend(); }
    // TODO constexpr const_reverse_iterator crbegin() const noexcept;
    // TODO constexpr const_reverse_iterator crend() const noexcept;

    //- Capacity
    //
    constexpr size_type capacity() const noexcept { return engine_.capacity(); }
    constexpr size_type elements() const noexcept { return engine_.elements(); }
    constexpr size_type size() const noexcept { return engine_.elements(); }
    constexpr void reserve([[maybe_unused]] size_type elemcap)
    {
        // static_assert(std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.reserve(elemcap);
    }
    constexpr void resize([[maybe_unused]] size_type elems)
    {
        reserve(elems);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.resize(elems);
    }
    constexpr void resize([[maybe_unused]] size_type elems, [[maybe_unused]] size_type elemcap)
    {
        //static_assert(std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>);
        if constexpr (std::is_same_v<typename engine_type::engine_category, resizable_vector_engine_tag>)
            engine_.resize(elems, elemcap);
    }

    //- Element access
    //
    constexpr reference operator[](size_type i) { return engine_(i); }
    constexpr reference operator()(size_type i) { return engine_(i); }
    constexpr const_reference operator[](size_type i) const { return engine_(i); }
    constexpr const_reference operator()(size_type i) const { return engine_(i); }
    constexpr transpose_type t() { return *this; }
    constexpr const_transpose_type t() const { return *this; }
    constexpr hermitian_type h()
    {
        auto ht = hermitian_type(size());
        size_t i = 0;
        for (auto && v : *this)
        {
            ht(i) = (*this)(i);
            ++i;
        }
        return ht;
    }
    constexpr const_hermitian_type h() const
    {
        auto ht = hermitian_type(size());
        size_t i = 0;
        for (auto && v : *this)
        {
            ht(i) = (*this)(i);
            ++i;
        }
        return ht;
    }

    //- Data access
    //
    constexpr engine_type& engine() noexcept;
    constexpr engine_type const& engine() const noexcept;

    //- Modifiers
    //
    constexpr void swap(vector& rhs) noexcept;
    constexpr void swap_elements(size_type i, size_type j) noexcept;
};
// }}}
// {{{ 6.5.2 | class matrix<ET, OT>
template <class ET, class OT>
class matrix {
  private:
    ET engine_{};

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
    using column_type = vector<column_engine<ET, TODO /*see below*/ >, OT>;
    using const_column_type = vector<column_engine<ET, readable_vector_engine_tag>, OT>;
    using row_type = vector<row_engine<ET, TODO /*see below*/ >, OT>;
    using const_row_type = vector<row_engine<ET, readable_vector_engine_tag>, OT>;
    using submatrix_type = matrix<submatrix_engine<ET, TODO /*see below*/ >, OT>;
    using const_submatrix_type = matrix<submatrix_engine<ET, readable_matrix_engine_tag>, OT>;
    using transpose_type = matrix<transpose_engine<ET, TODO /*see below*/ >, OT>;
    using const_transpose_type = matrix<transpose_engine<ET, readable_matrix_engine_tag>, OT>;
    using hermitian_type = std::conditional_t<util::is_complex_v<element_type>, matrix, transpose_type>;
    using const_hermitian_type = std::conditional_t<util::is_complex_v<element_type>, matrix, const_transpose_type>;

    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;
    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;
    template <typename U>
    constexpr matrix(std::initializer_list<U> list) : engine_(list) {}

    template <class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src) : engine_{src.engine_} {}
    constexpr matrix(size_tuple size) { resize(size); }
    constexpr matrix(size_type rows, size_type cols);
    constexpr matrix(size_tuple size, size_tuple cap);
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    constexpr matrix& operator=(matrix&&) noexcept = default;
    constexpr matrix& operator=(matrix const&) = default;
    template <class ET2, class OT2>
    constexpr matrix& operator=(matrix<ET2, OT2> const& rhs);

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return engine_.rows(); }
    constexpr size_type rows() const noexcept { return engine_.columns(); }
    constexpr size_tuple size() const noexcept { return engine_.size(); }
    constexpr size_type column_capacity() const noexcept { return engine_.column_capacity(); }
    constexpr size_type row_capacity() const noexcept { return engine_.row_capacity(); }
    constexpr size_tuple capacity() const noexcept { return engine_.capacity(); }
    constexpr void reserve(size_tuple cap) { engine_.reserve(cap); }
    constexpr void reserve(size_type rowcap, size_type colcap) { engine_.reserve(rowcap, colcap); }
    constexpr void resize(size_tuple size)
    {
        static_assert(std::is_same_v<engine_type::engine_category, resizable_matrix_engine_tag>);
        if constexpr (std::is_same_v<engine_type::engine_category, resizable_matrix_engine_tag>)
            engine_.resize(std::get<0>(size), std::get<1>(size));
    }
    constexpr void resize(size_type rows, size_type cols) { engine_.resize(rows, cols); }
    constexpr void resize(size_tuple size, size_tuple cap) { engine_.resize(size, cap); }
    constexpr void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
    {
        engine_.resize(rows, cols, rowcap, colcap);
    }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) { return engine_(i, j); }
    constexpr const_reference operator()(size_type i, size_type j) const { return engine_(i, j); }

    //- Columns, rows, submatrices, transposes, and the Hermitian
    //
    constexpr column_type column(size_type j) noexcept { return engine_.column(j); }
    constexpr const_column_type column(size_type j) const noexcept { return engine_.column(j); }
    constexpr row_type row(size_type i) noexcept { return engine_.row(i); }
    constexpr const_row_type row(size_type i) const noexcept { return engine_.row(i); }
    constexpr submatrix_type submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept {
        return engine_.submatrix(ri, rn, ci, cn);
    }
    constexpr const_submatrix_type submatrix(size_type ri, size_type rn,
                                             size_type ci, size_type cn) const noexcept {
        return engine_.submatrix(ri, rn, ci, cn);
    }
    constexpr transpose_type t() noexcept; // TODO
    constexpr const_transpose_type t() const noexcept; // TODO
    constexpr hermitian_type h(); // TODO
    constexpr const_hermitian_type h() const; // TODO

    //- Data access
    //
    constexpr engine_type& engine() noexcept { return engine_; }
    constexpr engine_type const& engine() const noexcept { return engine_; }

    //- Modifiers
    //
    constexpr void swap(matrix& rhs) noexcept { engine_.swap(rhs.engine_); }
    constexpr void swap_columns(size_type i, size_type j) noexcept { engine_.swap_columns(i, j); }
    constexpr void swap_rows(size_type i, size_type j) noexcept { engine_.swap_rows(i, j); }
};
// }}}
// {{{ 6.6.1 | matrix_operation_traits
template <class T1>
struct matrix_negation_element_traits { using element_type = decltype(-std::declval<T1>()); };

template <class OT, class T1>
using matrix_negation_element_t = typename OT::template element_negation_traits<T1>;

template <class T1, class T2>
struct matrix_addition_element_traits { using element_type = decltype(std::declval<T1>() + std::declval<T2>()); };

template <class OT, class T1, class T2>
using matrix_addition_element_t = typename OT::template element_addition_traits<T1, T2>;

template <class T1, class T2>
struct matrix_subtraction_element_traits { using element_type = decltype(std::declval<T1>() - std::declval<T2>()); };

template <class OT, class T1, class T2>
using matrix_subtraction_element_t = typename OT::template element_subtraction_traits<T1, T2>;

template <class T1, class T2>
struct matrix_multiplication_element_traits { using element_type = decltype(std::declval<T1>() * std::declval<T2>()); };

template <class OT, class T1, class T2>
using matrix_multiplication_element_t = typename OT::template element_multiplication_traits<T1, T2>::element_type;

// {{{ 6.8.1 | engine promotion traits | matrix_negation_engine_traits<OT, ET1>
template <class OT, class ET1>
struct matrix_negation_engine_traits // | 4.8.1
{
    using element_type = matrix_negation_element_t<OT, typename ET1::element_type>;
    using engine_type = ET1; // TODO; //- Implementation-defined
};

template <class OT, class ET1>
using matrix_negation_engine_t = typename OT::template engine_negation_traits<
    OT, // TODO: OTR
    ET1
>::engine_type; // XXX:DEFECT? the documentation is uncearl wrt. engine type vs TRAIT!
// }}}
// {{{ 6.8.2 | engine promotion traits | matrix_addition_engine_traits<OT, ET1, ET2>
template <class OT, class ET1, class ET2>
struct matrix_addition_engine_traits // | 4.8.2
{
    using element_type = matrix_addition_element_t<OT, typename ET1::element_type, typename ET2::element_type>;
    using engine_type = ET1; // TODO
};

template <class OT, class ET1, class ET2>
using matrix_addition_engine_t = typename OT::template engine_addition_traits<
    OT, // TODO: OTR
    ET1,
    ET2
>::engine_type;
// }}}
// {{{ 6.8.3 | engine promotion traits | matrix_subtraction_engine_traits<OT, ET1, ET2>
template <class OT, class ET1, class ET2>
struct matrix_subtraction_engine_traits
{
    using element_type = matrix_subtraction_element_t<OT, typename ET1::element_type, typename ET2::element_type>;
    using engine_type = ET1; // TODO
};

template <class OT, class ET1, class ET2>
using matrix_subtraction_engine_t = typename OT::template engine_subtraction_traits<
    OT, // TODO: OTR
    ET1,
    ET2
>::engine_type; // XXX:DEFECT? wrt `::engine_type`?
// }}}
// {{{ 6.8.4 | engine promotion traits | matrix_multiplication_engine_traits<OT, ET1, ET2>
template <class OT, class ET1, class ET2>
struct matrix_multiplication_engine_traits
{
    using element_type = matrix_multiplication_element_t<OT, typename ET1::element_type, typename ET2::element_type>;
    using engine_type = ET1; // TODO
};

template <class OT, class ET1, class ET2>
using matrix_multiplication_engine_t = typename OT::template element_multiplication_traits<ET1, ET2>;
// }}}
// {{{ 6.9.1 | matrix_negation_traits<OT, OP1, OP2>
template <class OT, class OP1> struct matrix_negation_traits;

template <class OT, class ET1, class OT1>
struct matrix_negation_traits<OT, vector<ET1, OT1>>
{
    using engine_type = matrix_negation_engine_t<OT, ET1>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type negate(vector<ET1, OT1> const& v1)
    {
        auto res = result_type(v1.size());
        size_t i = 0;
        for (auto const& v : v1)
            res(i++) = -v;
        return res;
    }
};

template <class OT, class ET1, class OT1>
struct matrix_negation_traits<OT, matrix<ET1, OT1>>
{
    using engine_type = matrix_negation_engine_t<OT, ET1>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type negate(matrix<ET1, OT1> const& m1)
    {
        result_type m{};

        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                m(i, j) = -m1(i, j);

        return m;
    }
};

template <class OT, class OP1>
using matrix_negation_traits_t = typename OT::template negation_traits<OT, OP1>;
// }}}
// {{{ 6.9.2 | matrix_addition_traits<OT, OP1, OP2>
template <class OT, class OP1, class OP2> struct matrix_addition_traits;

template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type add(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
    {
        result_type v3{};
        for (std::size_t i = 0; i < v1.size(); ++i)
            v3(i) = v1(i) + v2(i);
        return v3;
    }
};

template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_addition_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_addition_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type add(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        using size_type = std::size_t;
        result_type m{};

        for (size_type i = 0; i < m1.rows(); ++i)
            for (size_type j = 0; j < m1.columns(); ++j)
                m(i, j) = m1(i, j) + m2(i, j);

        return m;
    }
};

template <class OT, class OP1, class OP2>
using matrix_addition_traits_t =
    typename OT::template addition_traits<
        OT, // TODO: OTR!
        OP1,
        OP2
    >;
// }}}
// {{{ 6.9.3 | matrix_subtraction_traits<OT, OP1, OP2>
template <class OT, class OP1, class OP2> struct matrix_subtraction_traits;

template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type subtract(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
    {
        result_type v3{};
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            v3(i) = v1(i) - v2(i);
        return v3;
    }
};

template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_subtraction_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_subtraction_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type subtract(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        using size_type = std::size_t;
        result_type m{};

        for (size_type i = 0; i < m1.rows(); ++i)
            for (size_type j = 0; j < m1.columns(); ++j)
                m(i, j) = m1(i, j) - m2(i, j);

        return m;
    }
};

template <class OT, class OP1, class OP2>
using matrix_subtraction_traits_t =
    typename OT::template subtraction_traits<
        OT, // TODO: OTR!
        OP1,
        OP2
    >;
// }}}
// {{{ 6.9.4 matrix_multiplication_traits<OT, OP1, OP2>
template <class OT, class OP1, class OP2> struct matrix_multiplication_traits;

// vector * scalar
template <class OT, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, T2>
{
    using scalar_type = scalar_engine<T2>;
    using engine_type = matrix_multiplication_engine_t<OT, ET1, scalar_type>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(vector<ET1, OT1> const& v1, T2 const& s2)
    {
        result_type r(v1.size());
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            r(i) = v1(i) * s2;
        return r;
    }
};

// scalar * vector
template <class OT, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, T1, vector<ET2, OT2>>
{
    using scalar_type = scalar_engine<T1>;
    using engine_type = matrix_multiplication_engine_t<OT, scalar_type, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(T1 const& s1, vector<ET2, OT2> const& v2)
    {
        result_type r(v2.size());
        for (decltype(v2.size()) i = 0; i < v2.size(); ++i)
            r(i) = s1 * v2(i);
        return r;
    }
};

// matrix * scalar
template <class OT, class ET1, class OT1, class T2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, T2>
{
    using scalar_type = scalar_engine<T2>;
    using engine_type = matrix_multiplication_engine_t<OT, ET1, scalar_type>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, T2 const& s2)
    {
        result_type r(m1.size());
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                r(i, j) = m1(i, j) * s2;
        return r;
    }
};

// scalar * matrix
template <class OT, class T1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, T1, matrix<ET2, OT2>>
{
    using scalar_type = scalar_engine<T1>;
    using engine_type = matrix_multiplication_engine_t<OT, scalar_type, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(T1 const& s1, matrix<ET2, OT2> const& m2)
    {
        result_type r(m2.size());
        for (decltype(m2.rows()) i = 0; i < m2.rows(); ++i)
            for (decltype(m2.columns()) j = 0; j < m2.columns(); ++j)
                r(i, j) = s1 * m2(i, j);
        return r;
    }
};

// vector * vector
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, vector<ET2, OT2>>
{
    //- This specialization returns a scalar, and therefore does not compute an engine type.
    using op_traits = OT;
    using elem_type_1 = typename vector<ET1, OT1>::element_type;
    using elem_type_2 = typename vector<ET2, OT2>::element_type;
    using result_type = matrix_multiplication_element_t<op_traits, elem_type_1, elem_type_2>;
    constexpr static result_type multiply(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
    {
        result_type r{};
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            r += v1(i) * v2(i);
        return r;
    }
};

// matrix*vector
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, vector<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& m2)
    {
        result_type r(m1.columns());
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
        {
            typename engine_type::value_type bi{};
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                bi += m1(i, j) * m2(j);
            r(i) = bi;
        }
        return r;
    }
};

// vector * matrix
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, vector<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = vector<engine_type, op_traits>;
    constexpr static result_type multiply(vector<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        // assert(m1.size() == m2.rows());
        result_type r(m2.columns());
        for (decltype(m2.columns()) j = 0; j < m2.columns(); ++j)
        {
            typename engine_type::value_type bi{};
            for (decltype(m2.rows()) i = 0; i < m2.rows(); ++i)
                bi += m1(i) * m2(i, j);
            r(j) = bi;
        }
        return r;
    }
};

// matrix*matrix
template <class OT, class ET1, class OT1, class ET2, class OT2>
struct matrix_multiplication_traits<OT, matrix<ET1, OT1>, matrix<ET2, OT2>>
{
    using engine_type = matrix_multiplication_engine_t<OT, ET1, ET2>;
    using op_traits = OT;
    using result_type = matrix<engine_type, op_traits>;
    constexpr static result_type multiply(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
    {
        result_type r(m1.rows(), m2.columns());
        for (decltype(r.rows()) i = 0; i < r.rows(); ++i)
        {
            for (decltype(r.columns()) j = 0; j < r.columns(); ++j)
            {
                typename engine_type::value_type v{};
                for (decltype(m1.columns()) k = 0; k < m1.columns(); ++k)
                    v += m1(i, k) * m2(k, j);
                r(i, j) = v;
            }
        }
        return r;
    }
};

template <class OT, class OP1, class OP2>
using matrix_multiplication_traits_t =
    typename OT::template multiplication_traits<
        OT, // TODO: OTR!
        OP1,
        OP2
    >;
// }}}

// Default matrix operation traits for
// - element promotion,
// - engine promotion, and
// - arithmetic operation traits for the four basic arithmetic operations.
struct matrix_operation_traits
{
    //- Default element promotion traits.
    //
    template <class T1>
    using element_negation_traits = matrix_negation_element_traits<T1>;
    template <class T1, class T2>
    using element_addition_traits = matrix_addition_element_traits<T1, T2>;
    template <class T1, class T2>
    using element_subtraction_traits = matrix_subtraction_element_traits<T1, T2>;
    template <class T1, class T2>
    using element_multiplication_traits = matrix_multiplication_element_traits<T1, T2>;

    //- Default engine promotion traits.
    //
    template <class OTR, class ET1>
    using engine_negation_traits = matrix_negation_engine_traits<OTR, ET1>;
    template <class OTR, class ET1, class ET2>
    using engine_addition_traits = matrix_addition_engine_traits<OTR, ET1, ET2>;
    template <class OTR, class ET1, class ET2>
    using engine_subtraction_traits = matrix_subtraction_engine_traits<OTR, ET1, ET2>;
    template <class OTR, class ET1, class ET2>
    using engine_multiplication_traits = matrix_multiplication_engine_traits<OTR, ET1, ET2>;

    //- Default arithmetic operation traits.
    //
    template <class OTR, class OP1> // XXX:DEFECT?
    using negation_traits = matrix_negation_traits<OTR, OP1>; // XXX:DEFECT
    template <class OTR, class OP1, class OP2>
    using addition_traits = matrix_addition_traits<OTR, OP1, OP2>;
    template <class OTR, class OP1, class OP2>
    using subtraction_traits = matrix_subtraction_traits<OTR, OP1, OP2>;
    template <class OTR, class OP1, class OP2>
    using multiplication_traits = matrix_multiplication_traits<OTR, OP1, OP2>;
};
// }}}
// {{{ 6.6.2 | matrix_operation_traits_selector
// Primary template and expected specializations.
template <class T1, class T2> struct matrix_operation_traits_selector;

template <class T1>
struct matrix_operation_traits_selector<T1, T1> { using traits_type = T1; };

template <class T1>
struct matrix_operation_traits_selector<T1, matrix_operation_traits> { using traits_type = T1; };

template <class T1>
struct matrix_operation_traits_selector<matrix_operation_traits, T1> { using traits_type = T1; };

template <>
struct matrix_operation_traits_selector<matrix_operation_traits, matrix_operation_traits>
{
    using traits_type = matrix_operation_traits;
};

//- Convenience alias.
template <class T1, class T2>
using matrix_operation_traits_selector_t =
    typename matrix_operation_traits_selector<T1, T2>::traits_type;
// }}}
// {{{ 6.10 | arithmetic operators
//- Negation
//
template <class ET1, class OT1>
inline auto
constexpr operator-(vector<ET1, OT1> const& v1)
{
    using op1_type = vector<ET1, OT1>;
    using op_traits = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;
    return neg_traits::negate(v1);
}

template <class ET1, class OT1>
inline auto
constexpr operator-(matrix<ET1, OT1> const& m1)
{
    using op1_type = matrix<ET1, OT1>;
    using op_traits = OT1;
    using neg_traits = matrix_negation_traits_t<op_traits, op1_type>;
    return neg_traits::negate(m1);
}

//- Addition
//
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator+(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;
    return add_traits::add(v1, v2);
}

template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator+(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    static_assert(is_matrix_engine_v<ET1>);
    static_assert(is_matrix_engine_v<ET2>);

    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using add_traits = matrix_addition_traits_t<op_traits, op1_type, op2_type>;
    return add_traits::add(m1, m2);
}

//- Subtraction
//
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator-(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;
    return sub_traits::subtract(v1, v2);
}

template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator-(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using sub_traits = matrix_subtraction_traits_t<op_traits, op1_type, op2_type>;
    return sub_traits::subtract(m1, m2);
}

//- Multiplication
//- vector*scalar and scalar*vector
//
template <class ET1, class OT1, class S2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);
    using op_traits = OT1;
    using op1_type = vector<ET1, OT1>;
    using op2_type = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, s2);
}

template <class S1, class ET2, class OT2>
inline auto
constexpr operator*(S1 const& s1, vector<ET2, OT2> const& v2)
{
    static_assert(is_matrix_element_v<S1>);
    using op_traits = OT2;
    using op1_type = S1;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(s1, v2);
}

// matrix*scalar and scalar*matrix
template <class ET1, class OT1, class S2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, S2 const& s2)
{
    static_assert(is_matrix_element_v<S2>);
    using op_traits = OT1;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = S2;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, s2);
}

template <class S1, class ET2, class OT2>
inline auto
constexpr operator*(S1 const& s1, matrix<ET2, OT2> const& m2)
{
    static_assert(is_matrix_element_v<S1>);
    using op_traits = OT2;
    using op1_type = S1;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(s1, m2);
}

// vector*vector
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, v2);
}

// matrix*vector
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, vector<ET2, OT2> const& v2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = vector<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, v2);
}

// vector*matrix
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(vector<ET1, OT1> const& v1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = vector<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(v1, m2);
}

// matrix*matrix
template <class ET1, class OT1, class ET2, class OT2>
inline auto
constexpr operator*(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2)
{
    using op_traits = matrix_operation_traits_selector_t<OT1, OT2>;
    using op1_type = matrix<ET1, OT1>;
    using op2_type = matrix<ET2, OT2>;
    using mul_traits = matrix_multiplication_traits_t<op_traits, op1_type, op2_type>;
    return mul_traits::multiply(m1, m2);
}
// }}}
// {{{ Convenience aliases
template <class T, int32_t N>
using fs_vector = vector<fs_vector_engine<T, N>, matrix_operation_traits>;

template <class T, int32_t R, int32_t C>
using fs_matrix = matrix<fs_matrix_engine<T, R, C>, matrix_operation_traits>;
// }}}
// {{{ EXT
template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(vector<ET1, OT1> const& v1, vector<ET2, OT2> const& v2) noexcept
{
    // TODO: replace naive implementationbb:w, use of std::equal?
    if (v1.size() == v2.size())
        for (decltype(v1.size()) i = 0; i < v1.size(); ++i)
            if (!(v1(i) == v2(i)))
                return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator==(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    // TODO: replace naive implementationbb:w, use of std::equal?
    if (m1.rows() == m2.rows() && m1.columns() == m2.columns())
        for (decltype(m1.rows()) i = 0; i < m1.rows(); ++i)
            for (decltype(m1.columns()) j = 0; j < m1.columns(); ++j)
                if (!(m1(i, j) == m2(i, j)))
                    return false;

    return true;
}

template <typename ET1, typename OT1, typename ET2, typename OT2>
constexpr bool operator!=(matrix<ET1, OT1> const& m1, matrix<ET2, OT2> const& m2) noexcept
{
    return !(m1 == m2);
}
// }}}

} // end namespace

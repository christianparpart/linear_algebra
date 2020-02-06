#pragma once

#include <cstdint>
#include <algorithm>
#include <complex>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <type_traits>

namespace math {
// ---------------------------------------------------------------------------------
// 6.4.1 dr_vector_engine

template<class T, class AT>
class dr_vector_engine {
  public:
    //- Types
    //
    using engine_category = resizable_vector_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using allocator_type = AT;
    using pointer = typename std::allocator_traits<AT>::pointer;
    using const_pointer = typename std::allocator_traits<AT>::const_pointer;
    using reference = element_type&;

    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    struct iterator {}; // TODO ...; //- Implementation-defined
    struct const_iterator {}; // TODO ...; //- Implementation-defined
    //- Construct/copy/destroy
    //
    ~dr_vector_engine() noexcept;
    dr_vector_engine();
    dr_vector_engine(dr_vector_engine&&) noexcept;
    dr_vector_engine(dr_vector_engine const&);
    template<class U>
    dr_vector_engine(std::initializer_list<U> list);
    dr_vector_engine(size_type elems);
    dr_vector_engine(size_type elems, size_type elem_cap);
    dr_vector_engine& operator =(dr_vector_engine&& rhs) noexcept;
    dr_vector_engine& operator =(dr_vector_engine const& rhs);
    template<class ET2>
    dr_vector_engine& operator =(ET2 const& rhs);
    //- Iterators
    //
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    //- Capacity
    //
    size_type capacity() const noexcept;
    size_type elements() const noexcept;
    void reserve(size_type cap);
    void resize(size_type elems);
    void resize(size_type elems, size_type cap);
    //- Element access
    //
    reference operator ()(size_type i);
    const_reference operator ()(size_type i) const;
    //- Modifiers
    //
    void swap(dr_vector_engine& rhs) noexcept;
    void swap_elements(size_type i, size_type j) noexcept;
};

// ---------------------------------------------------------------------------------
// 6.4.2 dr_matrix_engine<T, AT>

template<class T, class AT>
class dr_matrix_engine {
  public:
    //- Types
    //
    using engine_category = resizable_matrix_engine_tag;
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using allocator_type = AT;
    using pointer = typename std::allocator_traits<AT>::pointer;
    using const_pointer = typename std::allocator_traits<AT>::const_pointer;
    using reference = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using size_tuple = std::tuple<size_type, size_type>;

    //- Construct/copy/destroy
    //
    ~dr_matrix_engine() noexcept;
    dr_matrix_engine();
    dr_matrix_engine(dr_matrix_engine&& rhs) noexcept;
    dr_matrix_engine(dr_matrix_engine const& rhs);
    dr_matrix_engine(size_type rows, size_type cols);
    dr_matrix_engine(size_type rows, size_type cols, size_type rowcap, size_type colcap);
    dr_matrix_engine& operator =(dr_matrix_engine&&) noexcept;
    dr_matrix_engine& operator =(dr_matrix_engine const&);
    template<class ET2>
    dr_matrix_engine& operator =(ET2 const& rhs);

    //- Capacity
    //
    size_type columns() const noexcept;
    size_type rows() const noexcept;
    size_tuple size() const noexcept;
    size_type column_capacity() const noexcept;
    size_type row_capacity() const noexcept;
    size_tuple capacity() const noexcept;
    void reserve(size_type rowcap, size_type colcap);
    void resize(size_type rows, size_type cols);
    void resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Element access
    //
    reference operator ()(size_type i, size_type j);
    const_reference operator ()(size_type i, size_type j) const;

    //- Modifiers
    //
    void swap(dr_matrix_engine& other) noexcept;
    void swap_columns(size_type c1, size_type c2) noexcept;
    void swap_rows(size_type r1, size_type r2) noexcept;
};

// ---------------------------------------------------------------------------------
// 6.4.3 fs_vector_engine

template<class T, size_t N>
class fs_vector_engine {
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
    struct iterator {}; // = ...; //- Implementation-defined
    struct const_iterator {}; // = ...; //- Implementation-defined
    //- Construct/copy/destroy
    //
    ~fs_vector_engine() noexcept = default;
    constexpr fs_vector_engine();
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;
    template<class U>
    constexpr fs_vector_engine(std::initializer_list<U> list);
    constexpr fs_vector_engine& operator =(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine& operator =(fs_vector_engine const&) = default;
    //- Iterators
    //
    constexpr iterator begin() noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    //- Capacity
    //
    static constexpr size_type capacity() noexcept;
    static constexpr size_type elements() noexcept;
    //- Element access
    //
    constexpr reference operator ()(size_type i);
    constexpr const_reference operator ()(size_type i) const;
    //- Modifiers
    //
    constexpr void swap(fs_vector_engine& rhs) noexcept;
    constexpr void swap_elements(size_type i, size_type j) noexcept;
};

// ---------------------------------------------------------------------------------
// 6.4.4 fs_matrix_engine

template<class T, size_t R, size_t C>
class fs_matrix_engine {
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
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using size_tuple = std::tuple<size_type, size_type>;

    //- Construct/copy/destroy
    //
    ~fs_matrix_engine() noexcept = default;
    constexpr fs_matrix_engine();
    template<class U>
    constexpr fs_matrix_engine(std::initializer_list<U> list);
    constexpr fs_matrix_engine(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine(fs_matrix_engine const&) = default;
    constexpr fs_matrix_engine& operator =(fs_matrix_engine&&) noexcept = default;
    constexpr fs_matrix_engine& operator =(fs_matrix_engine const&) = default;
    template<class ET2>
    constexpr fs_matrix_engine& operator =(ET2 const& rhs);

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
    constexpr reference operator ()(size_type i, size_type j);
    constexpr const_reference operator ()(size_type i, size_type j) const;

    //- Modifiers
    //
    constexpr void swap(fs_matrix_engine& rhs) noexcept;
    constexpr void swap_columns(size_type j1, size_type j2) noexcept;
    constexpr void swap_rows(size_type i1, size_type i2) noexcept;
};

// ---------------------------------------------------------------------------------
// 6.4.5 column_engine

template<class ET, class VCT>
class column_engine {
  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = ...; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = ...; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using iterator = ...; //- Implementation-defined
    using const_iterator = ...; //- Implementation-defined

    //- Construct/copy/destroy
    //
    ~column_engine() noexcept = default;
    constexpr column_engine() noexcept;
    constexpr column_engine(column_engine&&) noexcept = default;
    constexpr column_engine(column_engine const&) noexcept = default;
    constexpr column_engine& operator =(column_engine&&) noexcept = default;
    constexpr column_engine& operator =(column_engine const&) noexcept = default;

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
    constexpr reference operator ()(size_type i) const;

    //- Modifiers
    //
    constexpr void swap(column_engine& rhs);
};

// ----------------------------------------------------------------------------------------------
// 6.4.6 row_engine

template<class ET, class VCT>
class row_engine {
    static_assert(std::is_matrix_engine_v<ET>);
    static_assert(std::is_vector_engine_tag<VCT>);
  public:
    //- Types
    //
    using engine_category = VCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = ...; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = ...; //- See below
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using iterator = ...; //- Implementation-defined
    using const_iterator = ...; //- Implementation-defined
    //- Construct/copy/destroy
    //
    ~row_engine() noexcept = default;
    constexpr row_engine() noexcept;
    constexpr row_engine(row_engine&&) noexcept = default;
    constexpr row_engine(row_engine const&) noexcept = default;
    constexpr row_engine& operator =(row_engine&&) noexcept = default;
    constexpr row_engine& operator =(row_engine const&) noexcept = default;
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
    constexpr reference operator ()(size_type i) const;
    //- Modifiers
    //
    constexpr void swap(row_engine& rhs);
};


// ----------------------------------------------------------------------------------------------
// 6.4.7 transpose_engine

template<class ET, class MCT>
class transpose_engine {
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = ...; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = ...; //- See below
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
    constexpr transpose_engine& operator =(transpose_engine&&) noexcept = default;
    constexpr transpose_engine& operator =(transpose_engine const&) = default;
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
    constexpr reference operator ()(size_type i, size_type j) const;
    //- Modifiers
    //
    constexpr void swap(transpose_engine& rhs);
};

// ----------------------------------------------------------------------------------------------
//  6.4.8 submatrix_engine<ET, MCT>

template<class ET, class MCT>
class submatrix_engine {
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = ...; //- See below
    using const_pointer = typename ET::const_pointer;
    using reference = ...; //- See below
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
    constexpr submatrix_engine& operator =(submatrix_engine&&) noexcept = default;
    constexpr submatrix_engine& operator =(submatrix_engine const&) = default;
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
    constexpr reference operator ()(size_type i, size_type j) const;
    //- Modifiers
    //
    constexpr void swap(submatrix_engine& rhs);
};

} // end namespace

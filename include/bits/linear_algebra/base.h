#pragma once

#include <type_traits>
#include <complex>

#define LINEAR_ALGEBRA_NAMESPACE math

using TODO = void*; // XXX well, yeah ...

namespace LINEAR_ALGEBRA_NAMESPACE {

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

} // end namespace

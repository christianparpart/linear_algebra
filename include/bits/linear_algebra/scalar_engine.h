#pragma once

#include "base.h"
#include <type_traits>

namespace LINEAR_ALGEBRA_NAMESPACE {

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

} // end namespace

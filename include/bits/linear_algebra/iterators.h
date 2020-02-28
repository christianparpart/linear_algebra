#pragma once

#include "base.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

//- Iteration for vector.
template <typename ET, typename OT> constexpr TODO begin(vector<ET, OT>& v) noexcept;

template <typename ET, typename OT> constexpr TODO end(vector<ET, OT>& v) noexcept;

template <typename ET, typename OT> constexpr TODO begin(vector<ET, OT> const& v) noexcept;

template <typename ET, typename OT> constexpr TODO end(vector<ET, OT> const & v) noexcept;

template <typename ET, typename OT> constexpr TODO rbegin(vector<ET, OT> const& v) noexcept;

template <typename ET, typename OT> constexpr TODO rend(vector<ET, OT> const & v) noexcept;

template <typename ET, typename OT> constexpr TODO cbegin(vector<ET, OT> const& v) noexcept;

template <typename ET, typename OT> constexpr TODO cend(vector<ET, OT> const & v) noexcept;

template <typename ET, typename OT> constexpr TODO rbegin(vector<ET, OT>& v) noexcept;

template <typename ET, typename OT> constexpr TODO rend(vector<ET, OT>& v) noexcept;

template <typename ET, typename OT> constexpr TODO crbegin(vector<ET, OT> const& v) noexcept;

template <typename ET, typename OT> constexpr TODO crend(vector<ET, OT> const & v) noexcept;

} // end namespace

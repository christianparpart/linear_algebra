/**
 * This file is part of the "dim" project
 *   Copyright (c) 2020 Christian Parpart <christian@parpart.family>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "base.h"

namespace LINEAR_ALGEBRA_NAMESPACE {

// 6.4.6
template <typename ET, typename MCT>
class matrix_view_engine<ET, MCT, submatrix_view_tag>
{
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = std::conditional_t<is_writable_engine_v<ET>, value_type*, value_type const*>;
    using const_pointer = typename ET::const_pointer;
    using reference = std::conditional_t<is_writable_engine_v<ET>, value_type&, value_type const&>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;
    using span_type = TODO; // implementation-defined ; (see note )
    using const_span_type = TODO; // implementation-defined ; (see note )

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine const&) noexcept = default;
    template <typename ET2> constexpr matrix_view_engine& operator=(ET2 const& rhs);
    template <typename U> constexpr matrix_view_engine& operator=(std::initializer_list<std::initializer_list<U>> list);

    // EXT
    constexpr matrix_view_engine(ET* _engine, size_type ri, size_type rn, size_type ci, size_type cn) noexcept :
        engine_{_engine}, ri_{ri}, rn_{rn}, ci_{ci}, cn_{cn}
    {}

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return engine_->columns() - cn_; }
    constexpr size_type rows() const noexcept { return engine_->rows() - rn_; }
    constexpr size_tuple size() const noexcept { return { rows(), columns() }; }
    constexpr size_type column_capacity() const noexcept { return columns(); }
    constexpr size_type row_capacity() const noexcept { return rows(); }
    constexpr size_tuple capacity() const noexcept { return size(); }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const
    {
        assert(engine_ != nullptr);
        return i < ri_
            ? j < ci_
                ? (*engine_)(i, j)
                : (*engine_)(i, j + cn_)
            : j < ci_
                ? (*engine_)(i + rn_, j)
                : (*engine_)(i + rn_, j + cn_);
    }

    //- Data access
    //
    constexpr span_type span() const noexcept; // TODO

    //- Modifiers
    //
    constexpr void swap(matrix_view_engine& rhs)
    {
        std::swap(engine_, rhs.engine_);
        std::swap(ri_, rhs.ri_);
        std::swap(rn_, rhs.rn_);
        std::swap(ci_, rhs.ci_);
        std::swap(cn_, rhs.cn_);
    }

    // EXT
    constexpr std::vector<size_type> const& rowMapping() const noexcept { return rowMapping_; }
    constexpr std::vector<size_type> const& columnMapping() const noexcept { return columnMapping_; }

  private:
    ET* engine_ = nullptr;
    size_type ri_ = 0;
    size_type rn_ = 0;
    size_type ci_ = 0;
    size_type cn_ = 0;
    std::vector<size_type> rowMapping_;
    std::vector<size_type> columnMapping_;
};

template <typename T, size_t R, size_t C, typename MCT>
class matrix_view_engine<fs_matrix_engine<T, R, C>, MCT, submatrix_view_tag> {
    using ET = fs_matrix_engine<T, R, C>;
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = std::conditional_t<is_writable_engine_v<ET>, value_type*, value_type const*>;
    using const_pointer = typename ET::const_pointer;
    using reference = std::conditional_t<is_writable_engine_v<ET>, value_type&, value_type const&>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;
    using span_type = TODO; // implementation-defined ; (see note )
    using const_span_type = TODO; // implementation-defined ; (see note )

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine const&) noexcept = default;
    template <typename ET2> constexpr matrix_view_engine& operator=(ET2 const& rhs);
    template <typename U> constexpr matrix_view_engine& operator=(std::initializer_list<std::initializer_list<U>> list);

    // EXT
    constexpr matrix_view_engine(ET* _engine, size_type ri, size_type rn, size_type ci, size_type cn) noexcept :
        engine_{_engine},
        rowCount_{R - rn},
        columnCount_{C - cn}
    {
        printf("submatrix_engine<%zu, %zu>\n", R - rn, C - rn);
        using detail::times;
        for (size_type const i : times(R))
            rowMapping_[i] = i < ri ? i : i + rn;

        for (size_type const j : times(C))
            columnMapping_[j] = j < ci ? j : j + cn;
    }

    constexpr matrix_view_engine(ET* _engine, std::array<size_type, R> _rowMapping,
                                 std::array<size_type, C> _columnMapping) :
        engine_{_engine}, rowMapping_{_rowMapping}, columnMapping_{_columnMapping}
    {}

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return columnCount_; }
    constexpr size_type rows() const noexcept { return rowCount_; }
    constexpr size_tuple size() const noexcept { return { rows(), columns() }; }
    constexpr size_type column_capacity() const noexcept { return columns(); }
    constexpr size_type row_capacity() const noexcept { return rows(); }
    constexpr size_tuple capacity() const noexcept { return size(); }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const
    {
        size_type const xi = rowMapping_[i];
        size_type const xj = columnMapping_[j];
        return (*engine_)(xi, xj);
    }

    //- Data access
    //
    constexpr span_type span() const noexcept; // TODO

    //- Modifiers
    //
    constexpr void swap(matrix_view_engine& rhs)
    {
        std::swap(engine_, rhs.engine_);
        std::swap(rowMapping_, rhs.rowMapping_);
        std::swap(columnMapping_, rhs.columnMapping_);
    }

    // EXT
    constexpr std::array<size_type, R> const& rowMapping() const noexcept { return rowMapping_; }
    constexpr std::array<size_type, C> const& columnMapping() const noexcept { return columnMapping_; }

  private:
    ET* engine_ = nullptr;
    size_type rowCount_;
    size_type columnCount_;
    std::array<size_type, R> rowMapping_{};
    std::array<size_type, C> columnMapping_{};
};

template <typename T, size_t R, size_t C, typename MCT>
class matrix_view_engine<
    matrix_view_engine<
        fs_matrix_engine<T, R, C>, MCT,
        submatrix_view_tag
    >,
    MCT,
    submatrix_view_tag
> {
    using ET = matrix_view_engine<fs_matrix_engine<T, R, C>, MCT, submatrix_view_tag>;
  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type = typename ET::element_type;
    using value_type = typename ET::value_type;
    using pointer = std::conditional_t<is_writable_engine_v<ET>, value_type*, value_type const*>;
    using const_pointer = typename ET::const_pointer;
    using reference = std::conditional_t<is_writable_engine_v<ET>, value_type&, value_type const&>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type = typename ET::size_type;
    using size_tuple = typename ET::size_tuple;
    using span_type = TODO; // implementation-defined ; (see note )
    using const_span_type = TODO; // implementation-defined ; (see note )

    //- Construct/copy/destroy
    //
    ~matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine() noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine(matrix_view_engine const&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine&&) noexcept = default;
    constexpr matrix_view_engine& operator=(matrix_view_engine const&) noexcept = default;
    template <typename ET2> constexpr matrix_view_engine& operator=(ET2 const& rhs);
    template <typename U> constexpr matrix_view_engine& operator=(std::initializer_list<std::initializer_list<U>> list);

    // EXT
    constexpr matrix_view_engine(ET* _engine, size_type ri, size_type rn, size_type ci, size_type cn) noexcept :
        engine_{_engine},
        rowCount_{_engine->rows() - rn},
        columnCount_{_engine->columns() - cn},
        rowMapping_{_engine->rowMapping()},
        columnMapping_{_engine->columnMapping()}
    {
        using detail::times;
        for (size_type const i : times(_engine->rows()))
            rowMapping_[i] = i < ri ? i : i + rn;

        for (size_type const j : times(_engine->columns()))
            columnMapping_[j] = j < ci ? j : j + cn;
    }

    // EXT
    constexpr matrix_view_engine(ET* _engine,
                                 size_type ri,
                                 size_type rn,
                                 size_type ci,
                                 size_type cn,
                                 std::array<size_type, R> _rowMapping,
                                 std::array<size_type, C> _columnMapping) :
        engine_{_engine},
        rowCount_{_engine->rows() - rn},
        columnCount_{_engine->columns() - cn},
        rowMapping_{_rowMapping},
        columnMapping_{_columnMapping}
    {
        using detail::times;
        for (size_type const i : times(_engine->rows()))
            rowMapping_[i] = i < ri ? i : i + rn;

        for (size_type const j : times(_engine->columns()))
            columnMapping_[j] = j < ci ? j : j + cn;
    }

    //- Capacity
    //
    constexpr size_type columns() const noexcept { return columnCount_; }
    constexpr size_type rows() const noexcept { return rowCount_; }
    constexpr size_tuple size() const noexcept { return { rows(), columns() }; }
    constexpr size_type column_capacity() const noexcept { return columns(); }
    constexpr size_type row_capacity() const noexcept { return rows(); }
    constexpr size_tuple capacity() const noexcept { return size(); }

    //- Element access
    //
    constexpr reference operator()(size_type i, size_type j) const
    {
        size_type const xi = rowMapping_[i];
        size_type const xj = columnMapping_[j];
        return (*engine_)(xi, xj);
    }

    //- Data access
    //
    constexpr span_type span() const noexcept; // TODO

    //- Modifiers
    //
    constexpr void swap(matrix_view_engine& rhs)
    {
        std::swap(engine_, rhs.engine_);
        std::swap(rowMapping_, rhs.rowMapping_);
        std::swap(columnMapping_, rhs.columnMapping_);
    }

    // EXT
    constexpr std::array<size_type, R> const& rowMapping() const noexcept { return rowMapping_; }
    constexpr std::array<size_type, C> const& columnMapping() const noexcept { return columnMapping_; }

  private:
    ET* engine_ = nullptr;
    size_type rowCount_;
    size_type columnCount_;
    std::array<size_type, R> rowMapping_{};
    std::array<size_type, C> columnMapping_{};
};

} // end namespace

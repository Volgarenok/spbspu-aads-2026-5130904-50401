#ifndef MATRIX_ROW_CONST_ITER_HPP
#define MATRIX_ROW_CONST_ITER_HPP

#include <cstddef>

#include "matrix-row-iter.hpp"

namespace lukashevich {
  class Matrix;

  class MatrixRowConstIterator
  {
  public:
    MatrixRowConstIterator();
    MatrixRowConstIterator(const MatrixRowIterator & rhs) noexcept;

    MatrixRowConstIterator & operator++() noexcept;
    MatrixRowConstIterator operator++(int) noexcept;
    bool operator==(const MatrixRowConstIterator & rhs) const noexcept;
    bool operator!=(const MatrixRowConstIterator & rhs) const noexcept;

    const double & operator*() const;

  private:
    MatrixRowConstIterator(const Matrix * matrix, size_t row, size_t col) noexcept;

    const Matrix * matrix_;
    size_t row_;
    size_t col_;

    friend class Matrix;
  };
}

#endif

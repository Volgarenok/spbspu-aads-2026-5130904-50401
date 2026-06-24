#ifndef MATRIX_COL_CONST_ITER_HPP
#define MATRIX_COL_CONST_ITER_HPP

#include <cstddef>

#include "matrix-col-iter.hpp"

namespace lukashevich {
  class Matrix;

  class MatrixColConstIterator
  {
  public:
    MatrixColConstIterator();
    MatrixColConstIterator(const MatrixColIterator & rhs) noexcept;

    MatrixColConstIterator & operator++() noexcept;
    MatrixColConstIterator operator++(int) noexcept;
    bool operator==(const MatrixColConstIterator & rhs) const noexcept;
    bool operator!=(const MatrixColConstIterator & rhs) const noexcept;

    const double & operator*() const;

  private:
    MatrixColConstIterator(const Matrix * matrix, size_t row, size_t col) noexcept;

    const Matrix * matrix_;
    size_t row_;
    size_t col_;

    friend class Matrix;
  };
}

#endif

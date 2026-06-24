#ifndef MATRIX_DIAG_CONST_ITER_HPP
#define MATRIX_DIAG_CONST_ITER_HPP

#include <cstddef>

#include "matrix-diag-iter.hpp"

namespace lukashevich {
  class Matrix;

  class MatrixDiagConstIterator
  {
  public:
    MatrixDiagConstIterator();
    MatrixDiagConstIterator(const MatrixDiagIterator & rhs) noexcept;

    MatrixDiagConstIterator & operator++() noexcept;
    MatrixDiagConstIterator operator++(int) noexcept;

    bool operator==(const MatrixDiagConstIterator & rhs) const noexcept;
    bool operator!=(const MatrixDiagConstIterator & rhs) const noexcept;

    const double & operator*() const;

  private:
    MatrixDiagConstIterator(const Matrix * matrix,
        size_t index,
        bool isMainDiagonal) noexcept;

    const Matrix * matrix_;
    size_t index_;
    bool isMainDiagonal_;

    friend class Matrix;
  };
}

#endif

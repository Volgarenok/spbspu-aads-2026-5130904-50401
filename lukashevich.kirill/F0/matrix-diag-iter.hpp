#ifndef MATRIX_DIAG_ITER_HPP
#define MATRIX_DIAG_ITER_HPP

#include <cstddef>

namespace lukashevich {
  class Matrix;
  class MatrixDiagConstIterator;

  class MatrixDiagIterator
  {
  public:
    MatrixDiagIterator();

    MatrixDiagIterator & operator++() noexcept;
    MatrixDiagIterator operator++(int) noexcept;

    bool operator==(const MatrixDiagIterator & rhs) const noexcept;
    bool operator!=(const MatrixDiagIterator & rhs) const noexcept;

    double & operator*() const;

  private:
    MatrixDiagIterator(Matrix * matrix,
        size_t index,
        bool isMainDiagonal) noexcept;

    Matrix * matrix_;
    size_t index_;
    bool isMainDiagonal_;

    friend class Matrix;
    friend class MatrixDiagConstIterator;
  };
}

#endif

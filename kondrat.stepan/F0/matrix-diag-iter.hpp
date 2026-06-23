#ifndef MATRIX_DIAG_ITER_HPP
#define MATRIX_DIAG_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixDiagConstIter;

  class MatrixDiagIter
  {
    public:
      MatrixDiagIter() noexcept;

      long long & operator*() const noexcept;
      long long * operator->() const noexcept;

      MatrixDiagIter & operator++() noexcept;
      MatrixDiagIter operator++(int) noexcept;

      bool operator==(const MatrixDiagIter & other) const noexcept;
      bool operator!=(const MatrixDiagIter & other) const noexcept;

    private:
      friend class Matrix;
      friend class MatrixDiagConstIter;

      Matrix * matrix_;
      size_t index_;
      bool sideDiag_;

      MatrixDiagIter(Matrix * matrix, size_t index, bool sideDiag) noexcept;
  };
}

#endif

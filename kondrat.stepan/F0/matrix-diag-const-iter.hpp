#ifndef MATRIX_DIAG_CONST_ITER_HPP
#define MATRIX_DIAG_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixDiagIter;

  class MatrixDiagConstIter
  {
    public:
      MatrixDiagConstIter() noexcept;
      MatrixDiagConstIter(const MatrixDiagIter & other) noexcept;

      const long long & operator*() const noexcept;
      const long long * operator->() const noexcept;

      MatrixDiagConstIter & operator++() noexcept;
      MatrixDiagConstIter operator++(int) noexcept;

      bool operator==(const MatrixDiagConstIter & other) const noexcept;
      bool operator!=(const MatrixDiagConstIter & other) const noexcept;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t index_;
      bool sideDiag_;

      MatrixDiagConstIter(const Matrix * matrix, size_t index, bool sideDiag) noexcept;
  };
}

#endif

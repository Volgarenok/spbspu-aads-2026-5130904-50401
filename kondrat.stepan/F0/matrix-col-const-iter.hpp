#ifndef MATRIX_COL_CONST_ITER_HPP
#define MATRIX_COL_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixColIter;

  class MatrixColConstIter
  {
    public:
      MatrixColConstIter() noexcept;
      MatrixColConstIter(const MatrixColIter & other) noexcept;

      const long long & operator*() const noexcept;
      const long long * operator->() const noexcept;

      MatrixColConstIter & operator++() noexcept;
      MatrixColConstIter operator++(int) noexcept;

      bool operator==(const MatrixColConstIter & other) const noexcept;
      bool operator!=(const MatrixColConstIter & other) const noexcept;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixColConstIter(const Matrix * matrix, size_t row, size_t col) noexcept;
  };
}

#endif

#ifndef MATRIX_COL_ITER_HPP
#define MATRIX_COL_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixColConstIter;

  class MatrixColIter
  {
    public:
      MatrixColIter() noexcept;

      long long & operator*() const noexcept;
      long long * operator->() const noexcept;

      MatrixColIter & operator++() noexcept;
      MatrixColIter operator++(int) noexcept;

      bool operator==(const MatrixColIter & other) const noexcept;
      bool operator!=(const MatrixColIter & other) const noexcept;

    private:
      friend class Matrix;
      friend class MatrixColConstIter;

      Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixColIter(Matrix * matrix, size_t row, size_t col) noexcept;
  };
}

#endif

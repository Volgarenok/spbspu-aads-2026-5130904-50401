#ifndef MATRIX_ROW_CONST_ITER_HPP
#define MATRIX_ROW_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixRowIter;

  class MatrixRowConstIter
  {
    public:
      MatrixRowConstIter() noexcept;
      MatrixRowConstIter(const MatrixRowIter & other) noexcept;

      const long long & operator*() const noexcept;
      const long long * operator->() const noexcept;

      MatrixRowConstIter & operator++() noexcept;
      MatrixRowConstIter operator++(int) noexcept;

      bool operator==(const MatrixRowConstIter & other) const noexcept;
      bool operator!=(const MatrixRowConstIter & other) const noexcept;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixRowConstIter(const Matrix * matrix, size_t row, size_t col) noexcept;
  };
}

#endif

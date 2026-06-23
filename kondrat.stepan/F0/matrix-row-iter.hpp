#ifndef MATRIX_ROW_ITER_HPP
#define MATRIX_ROW_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixRowConstIter;

  class MatrixRowIter
  {
    public:
      MatrixRowIter() noexcept;

      long long & operator*() const noexcept;
      long long * operator->() const noexcept;

      MatrixRowIter & operator++() noexcept;
      MatrixRowIter operator++(int) noexcept;

      bool operator==(const MatrixRowIter & other) const noexcept;
      bool operator!=(const MatrixRowIter & other) const noexcept;

    private:
      friend class Matrix;
      friend class MatrixRowConstIter;

      Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixRowIter(Matrix * matrix, size_t row, size_t col) noexcept;
  };
}

#endif

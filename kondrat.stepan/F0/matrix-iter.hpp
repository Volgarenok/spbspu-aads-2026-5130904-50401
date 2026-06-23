#ifndef MATRIX_ITER_HPP
#define MATRIX_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixConstIter;

  class MatrixIter
  {
    public:
      MatrixIter() noexcept;

      long long & operator*() const noexcept;
      long long * operator->() const noexcept;

      MatrixIter & operator++() noexcept;
      MatrixIter operator++(int) noexcept;

      bool operator==(const MatrixIter & other) const noexcept;
      bool operator!=(const MatrixIter & other) const noexcept;

    private:
      friend class Matrix;
      friend class MatrixConstIter;

      Matrix * matrix_;
      size_t index_;

      MatrixIter(Matrix * matrix, size_t index) noexcept;
  };
}

#endif

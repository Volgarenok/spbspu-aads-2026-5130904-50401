#ifndef MATRIX_CONST_ITER_HPP
#define MATRIX_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixIter;

  class MatrixConstIter
  {
    public:
      MatrixConstIter() noexcept;
      MatrixConstIter(const MatrixIter & other) noexcept;

      const long long & operator*() const noexcept;
      const long long * operator->() const noexcept;

      MatrixConstIter & operator++() noexcept;
      MatrixConstIter operator++(int) noexcept;

      bool operator==(const MatrixConstIter & other) const noexcept;
      bool operator!=(const MatrixConstIter & other) const noexcept;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t index_;

      MatrixConstIter(const Matrix * matrix, size_t index) noexcept;
  };
}

#endif

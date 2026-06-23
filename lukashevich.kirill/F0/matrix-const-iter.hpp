#ifndef MATRIX_CONST_ITER_HPP
#define MATRIX_CONST_ITER_HPP

#include <cstddef>

#include "matrix-iter.hpp"

namespace lukashevich {
  class Matrix;

  class MatrixConstIterator
  {
  public:
    MatrixConstIterator();
    MatrixConstIterator(const MatrixIterator & rhs) noexcept;

    MatrixConstIterator & operator++() noexcept;
    MatrixConstIterator operator++(int) noexcept;
    bool operator==(const MatrixConstIterator & rhs) const noexcept;
    bool operator!=(const MatrixConstIterator & rhs) const noexcept;

    const double & operator*() const;

  private:
    MatrixConstIterator(const Matrix * matrix, size_t index) noexcept;

    const Matrix * matrix_;
    size_t index_;

    friend class Matrix;
  };
}

#endif

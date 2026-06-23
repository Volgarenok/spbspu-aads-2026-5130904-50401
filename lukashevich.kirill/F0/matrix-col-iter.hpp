#ifndef MATRIX_COL_ITER_HPP
#define MATRIX_COL_ITER_HPP

#include <cstddef>

namespace lukashevich {
  class Matrix;
  class MatrixColConstIterator;

  class MatrixColIterator
  {
  public:
    MatrixColIterator();

    MatrixColIterator & operator++() noexcept;
    MatrixColIterator operator++(int) noexcept;
    bool operator==(const MatrixColIterator & rhs) const noexcept;
    bool operator!=(const MatrixColIterator & rhs) const noexcept;

    double & operator*() const;

  private:
    MatrixColIterator(Matrix * matrix, size_t row, size_t col) noexcept;

    Matrix * matrix_;
    size_t row_;
    size_t col_;

    friend class Matrix;
    friend class MatrixColConstIterator;
  };
}

#endif

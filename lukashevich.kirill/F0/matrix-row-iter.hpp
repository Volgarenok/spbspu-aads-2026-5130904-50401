#ifndef MATRIX_ROW_ITER_HPP
#define MATRIX_ROW_ITER_HPP

#include <cstddef>

namespace lukashevich {
  class Matrix;
  class MatrixRowConstIterator;

  class MatrixRowIterator
  {
  public:
    MatrixRowIterator();

    MatrixRowIterator & operator++() noexcept;
    MatrixRowIterator operator++(int) noexcept;
    bool operator==(const MatrixRowIterator & rhs) const noexcept;
    bool operator!=(const MatrixRowIterator & rhs) const noexcept;

    double & operator*() const;

  private:
    MatrixRowIterator(Matrix * matrix, size_t row, size_t col) noexcept;

    Matrix * matrix_;
    size_t row_;
    size_t col_;

    friend class Matrix;
    friend class MatrixRowConstIterator;
  };
}

#endif

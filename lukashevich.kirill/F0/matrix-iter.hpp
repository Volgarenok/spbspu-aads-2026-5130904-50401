#ifndef MATRIX_ITER_HPP
#define MATRIX_ITER_HPP

#include <cstddef>

namespace lukashevich {
  class Matrix;
  class MatrixConstIterator;

  class MatrixIterator
  {
  public:
    MatrixIterator();

    MatrixIterator & operator++() noexcept;
    MatrixIterator operator++(int) noexcept;
    bool operator==(const MatrixIterator & rhs) const noexcept;
    bool operator!=(const MatrixIterator & rhs) const noexcept;

    double & operator*() const;

  private:
    MatrixIterator(Matrix * matrix, size_t index) noexcept;

    Matrix * matrix_;
    size_t index_;

    friend class Matrix;
    friend class MatrixConstIterator;
  };
}

#endif

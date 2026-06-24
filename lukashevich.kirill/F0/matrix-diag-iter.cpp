#include "matrix-diag-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixDiagIterator::MatrixDiagIterator():
  matrix_(nullptr),
  index_(0),
  isMainDiagonal_(true)
{}

lukashevich::MatrixDiagIterator::MatrixDiagIterator(Matrix * matrix,
    size_t index,
    bool isMainDiagonal) noexcept:
  matrix_(matrix),
  index_(index),
  isMainDiagonal_(isMainDiagonal)
{}

lukashevich::MatrixDiagIterator & lukashevich::MatrixDiagIterator::operator++()
    noexcept
{
  ++index_;
  return *this;
}

lukashevich::MatrixDiagIterator lukashevich::MatrixDiagIterator::operator++(int)
    noexcept
{
  MatrixDiagIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixDiagIterator::operator==(
    const MatrixDiagIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_)
      && (index_ == rhs.index_)
      && (isMainDiagonal_ == rhs.isMainDiagonal_);
}

bool lukashevich::MatrixDiagIterator::operator!=(
    const MatrixDiagIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

double & lukashevich::MatrixDiagIterator::operator*() const
{
  if (isMainDiagonal_) {
    return (*matrix_)(index_, index_);
  }

  return (*matrix_)(index_, matrix_->getCols() - index_ - 1);
}

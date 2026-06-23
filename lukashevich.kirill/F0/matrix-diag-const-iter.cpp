#include "matrix-diag-const-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixDiagConstIterator::MatrixDiagConstIterator():
  matrix_(nullptr),
  index_(0),
  isMainDiagonal_(true)
{}

lukashevich::MatrixDiagConstIterator::MatrixDiagConstIterator(
    const MatrixDiagIterator & rhs) noexcept:
  matrix_(rhs.matrix_),
  index_(rhs.index_),
  isMainDiagonal_(rhs.isMainDiagonal_)
{}

lukashevich::MatrixDiagConstIterator::MatrixDiagConstIterator(
    const Matrix * matrix,
    size_t index,
    bool isMainDiagonal) noexcept:
  matrix_(matrix),
  index_(index),
  isMainDiagonal_(isMainDiagonal)
{}

lukashevich::MatrixDiagConstIterator &
    lukashevich::MatrixDiagConstIterator::operator++() noexcept
{
  ++index_;
  return *this;
}

lukashevich::MatrixDiagConstIterator
    lukashevich::MatrixDiagConstIterator::operator++(int) noexcept
{
  MatrixDiagConstIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixDiagConstIterator::operator==(
    const MatrixDiagConstIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_)
      && (index_ == rhs.index_)
      && (isMainDiagonal_ == rhs.isMainDiagonal_);
}

bool lukashevich::MatrixDiagConstIterator::operator!=(
    const MatrixDiagConstIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

const double & lukashevich::MatrixDiagConstIterator::operator*() const
{
  if (isMainDiagonal_) {
    return (*matrix_)(index_, index_);
  }

  return (*matrix_)(index_, matrix_->getCols() - index_ - 1);
}

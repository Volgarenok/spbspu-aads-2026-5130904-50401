#include "matrix-const-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixConstIterator::MatrixConstIterator():
  matrix_(nullptr),
  index_(0)
{}

lukashevich::MatrixConstIterator::MatrixConstIterator(const MatrixIterator & rhs) noexcept:
  matrix_(rhs.matrix_),
  index_(rhs.index_)
{}

lukashevich::MatrixConstIterator::MatrixConstIterator(const Matrix * matrix,
    size_t index) noexcept:
  matrix_(matrix),
  index_(index)
{}

lukashevich::MatrixConstIterator & lukashevich::MatrixConstIterator::operator++() noexcept
{
  ++index_;
  return *this;
}

lukashevich::MatrixConstIterator lukashevich::MatrixConstIterator::operator++(int) noexcept
{
  MatrixConstIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixConstIterator::operator==(
    const MatrixConstIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (index_ == rhs.index_);
}

bool lukashevich::MatrixConstIterator::operator!=(
    const MatrixConstIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

const double & lukashevich::MatrixConstIterator::operator*() const
{
  return (*matrix_)(index_ / matrix_->getCols(), index_ % matrix_->getCols());
}

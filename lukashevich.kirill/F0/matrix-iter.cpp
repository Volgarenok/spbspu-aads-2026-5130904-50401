#include "matrix-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixIterator::MatrixIterator():
  matrix_(nullptr),
  index_(0)
{}

lukashevich::MatrixIterator::MatrixIterator(Matrix * matrix, size_t index) noexcept:
  matrix_(matrix),
  index_(index)
{}

lukashevich::MatrixIterator & lukashevich::MatrixIterator::operator++() noexcept
{
  ++index_;
  return *this;
}

lukashevich::MatrixIterator lukashevich::MatrixIterator::operator++(int) noexcept
{
  MatrixIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixIterator::operator==(const MatrixIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (index_ == rhs.index_);
}

bool lukashevich::MatrixIterator::operator!=(const MatrixIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

double & lukashevich::MatrixIterator::operator*() const
{
  return (*matrix_)(index_ / matrix_->getCols(), index_ % matrix_->getCols());
}

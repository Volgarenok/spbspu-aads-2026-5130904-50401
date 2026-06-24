#include "matrix-col-const-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixColConstIterator::MatrixColConstIterator():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

lukashevich::MatrixColConstIterator::MatrixColConstIterator(
  const MatrixColIterator & rhs) noexcept:
  matrix_(rhs.matrix_),
  row_(rhs.row_),
  col_(rhs.col_)
{}

lukashevich::MatrixColConstIterator::MatrixColConstIterator(const Matrix * matrix,
    size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

lukashevich::MatrixColConstIterator & lukashevich::MatrixColConstIterator::operator++() noexcept
{
  ++row_;
  return *this;
}

lukashevich::MatrixColConstIterator
    lukashevich::MatrixColConstIterator::operator++(int) noexcept
{
  MatrixColConstIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixColConstIterator::operator==(
    const MatrixColConstIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (row_ == rhs.row_) && (col_ == rhs.col_);
}

bool lukashevich::MatrixColConstIterator::operator!=(
    const MatrixColConstIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

const double & lukashevich::MatrixColConstIterator::operator*() const
{
  return (*matrix_)(row_, col_);
}

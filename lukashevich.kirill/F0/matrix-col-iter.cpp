#include "matrix-col-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixColIterator::MatrixColIterator():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

lukashevich::MatrixColIterator::MatrixColIterator(Matrix * matrix,
    size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

lukashevich::MatrixColIterator & lukashevich::MatrixColIterator::operator++() noexcept
{
  ++row_;
  return *this;
}

lukashevich::MatrixColIterator lukashevich::MatrixColIterator::operator++(int) noexcept
{
  MatrixColIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixColIterator::operator==(
    const MatrixColIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (row_ == rhs.row_) && (col_ == rhs.col_);
}

bool lukashevich::MatrixColIterator::operator!=(
    const MatrixColIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

double & lukashevich::MatrixColIterator::operator*() const
{
  return (*matrix_)(row_, col_);
}

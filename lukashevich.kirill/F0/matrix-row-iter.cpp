#include "matrix-row-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixRowIterator::MatrixRowIterator():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

lukashevich::MatrixRowIterator::MatrixRowIterator(Matrix * matrix,
    size_t row,
    size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

lukashevich::MatrixRowIterator & lukashevich::MatrixRowIterator::operator++()
    noexcept
{
  ++col_;
  return *this;
}

lukashevich::MatrixRowIterator lukashevich::MatrixRowIterator::operator++(int)
    noexcept
{
  MatrixRowIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixRowIterator::operator==(
    const MatrixRowIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (row_ == rhs.row_) && (col_ == rhs.col_);
}

bool lukashevich::MatrixRowIterator::operator!=(
    const MatrixRowIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

double & lukashevich::MatrixRowIterator::operator*() const
{
  return (*matrix_)(row_, col_);
}

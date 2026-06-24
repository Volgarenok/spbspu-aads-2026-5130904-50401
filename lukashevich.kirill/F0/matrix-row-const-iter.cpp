#include "matrix-row-const-iter.hpp"

#include "matrix.hpp"

lukashevich::MatrixRowConstIterator::MatrixRowConstIterator():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

lukashevich::MatrixRowConstIterator::MatrixRowConstIterator(const MatrixRowIterator & rhs) noexcept:
  matrix_(rhs.matrix_),
  row_(rhs.row_),
  col_(rhs.col_)
{}

lukashevich::MatrixRowConstIterator::MatrixRowConstIterator(const Matrix * matrix,
    size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

lukashevich::MatrixRowConstIterator & lukashevich::MatrixRowConstIterator::operator++() noexcept
{
  ++col_;
  return *this;
}

lukashevich::MatrixRowConstIterator lukashevich::MatrixRowConstIterator::operator++(int) noexcept
{
  MatrixRowConstIterator temp = *this;
  ++(*this);
  return temp;
}

bool lukashevich::MatrixRowConstIterator::operator==(const MatrixRowConstIterator & rhs) const noexcept
{
  return (matrix_ == rhs.matrix_) && (row_ == rhs.row_) && (col_ == rhs.col_);
}

bool lukashevich::MatrixRowConstIterator::operator!=(const MatrixRowConstIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

const double & lukashevich::MatrixRowConstIterator::operator*() const
{
  return (*matrix_)(row_, col_);
}

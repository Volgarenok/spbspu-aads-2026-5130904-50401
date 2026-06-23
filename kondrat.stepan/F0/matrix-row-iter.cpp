#include "matrix-row-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixRowIter::MatrixRowIter() noexcept:
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixRowIter::MatrixRowIter(Matrix * matrix, size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

long long & kondrat::MatrixRowIter::operator*() const noexcept
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

long long * kondrat::MatrixRowIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixRowIter & kondrat::MatrixRowIter::operator++() noexcept
{
  ++col_;
  return *this;
}

kondrat::MatrixRowIter kondrat::MatrixRowIter::operator++(int) noexcept
{
  MatrixRowIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixRowIter::operator==(const MatrixRowIter & other) const noexcept
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixRowIter::operator!=(const MatrixRowIter & other) const noexcept
{
  return !(*this == other);
}

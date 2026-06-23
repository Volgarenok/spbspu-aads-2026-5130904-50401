#include "matrix-col-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixColIter::MatrixColIter() noexcept:
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixColIter::MatrixColIter(Matrix * matrix, size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

long long & kondrat::MatrixColIter::operator*() const noexcept
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

long long * kondrat::MatrixColIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixColIter & kondrat::MatrixColIter::operator++() noexcept
{
  ++row_;
  return *this;
}

kondrat::MatrixColIter kondrat::MatrixColIter::operator++(int) noexcept
{
  MatrixColIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixColIter::operator==(const MatrixColIter & other) const noexcept
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixColIter::operator!=(const MatrixColIter & other) const noexcept
{
  return !(*this == other);
}

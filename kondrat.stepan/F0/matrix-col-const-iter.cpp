#include "matrix-col-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-col-iter.hpp"

kondrat::MatrixColConstIter::MatrixColConstIter() noexcept:
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixColConstIter::MatrixColConstIter(const MatrixColIter & other) noexcept:
  matrix_(other.matrix_),
  row_(other.row_),
  col_(other.col_)
{}

kondrat::MatrixColConstIter::MatrixColConstIter(const Matrix * matrix, size_t row, size_t col) noexcept:
  matrix_(matrix),
  row_(row),
  col_(col)
{}

const long long & kondrat::MatrixColConstIter::operator*() const noexcept
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

const long long * kondrat::MatrixColConstIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixColConstIter & kondrat::MatrixColConstIter::operator++() noexcept
{
  ++row_;
  return *this;
}

kondrat::MatrixColConstIter kondrat::MatrixColConstIter::operator++(int) noexcept
{
  MatrixColConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixColConstIter::operator==(const MatrixColConstIter & other) const noexcept
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixColConstIter::operator!=(const MatrixColConstIter & other) const noexcept
{
  return !(*this == other);
}

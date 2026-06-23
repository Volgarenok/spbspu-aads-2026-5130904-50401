#include "matrix-diag-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixDiagIter::MatrixDiagIter() noexcept:
  matrix_(nullptr),
  index_(0),
  sideDiag_(false)
{}

kondrat::MatrixDiagIter::MatrixDiagIter(Matrix * matrix, size_t index, bool sideDiag) noexcept:
  matrix_(matrix),
  index_(index),
  sideDiag_(sideDiag)
{}

long long & kondrat::MatrixDiagIter::operator*() const noexcept
{
  const size_t col = sideDiag_ ? matrix_->cols_ - index_ - 1 : index_;
  return matrix_->data_[index_ * matrix_->cols_ + col];
}

long long * kondrat::MatrixDiagIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixDiagIter & kondrat::MatrixDiagIter::operator++() noexcept
{
  ++index_;
  return *this;
}

kondrat::MatrixDiagIter kondrat::MatrixDiagIter::operator++(int) noexcept
{
  MatrixDiagIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixDiagIter::operator==(const MatrixDiagIter & other) const noexcept
{
  return matrix_ == other.matrix_ && index_ == other.index_ && sideDiag_ == other.sideDiag_;
}

bool kondrat::MatrixDiagIter::operator!=(const MatrixDiagIter & other) const noexcept
{
  return !(*this == other);
}

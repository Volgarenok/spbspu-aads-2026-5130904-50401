#include "matrix-diag-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-diag-iter.hpp"

kondrat::MatrixDiagConstIter::MatrixDiagConstIter() noexcept:
  matrix_(nullptr),
  index_(0),
  sideDiag_(false)
{}

kondrat::MatrixDiagConstIter::MatrixDiagConstIter(const MatrixDiagIter & other) noexcept:
  matrix_(other.matrix_),
  index_(other.index_),
  sideDiag_(other.sideDiag_)
{}

kondrat::MatrixDiagConstIter::MatrixDiagConstIter(
  const Matrix * matrix,
  size_t index,
  bool sideDiag) noexcept:
  matrix_(matrix),
  index_(index),
  sideDiag_(sideDiag)
{}

const long long & kondrat::MatrixDiagConstIter::operator*() const noexcept
{
  const size_t col = sideDiag_ ? matrix_->cols_ - index_ - 1 : index_;
  return matrix_->data_[index_ * matrix_->cols_ + col];
}

const long long * kondrat::MatrixDiagConstIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixDiagConstIter & kondrat::MatrixDiagConstIter::operator++() noexcept
{
  ++index_;
  return *this;
}

kondrat::MatrixDiagConstIter kondrat::MatrixDiagConstIter::operator++(int) noexcept
{
  MatrixDiagConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixDiagConstIter::operator==(const MatrixDiagConstIter & other) const noexcept
{
  return matrix_ == other.matrix_ && index_ == other.index_ && sideDiag_ == other.sideDiag_;
}

bool kondrat::MatrixDiagConstIter::operator!=(const MatrixDiagConstIter & other) const noexcept
{
  return !(*this == other);
}

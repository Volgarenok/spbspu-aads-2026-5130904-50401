#include "matrix-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-iter.hpp"

kondrat::MatrixConstIter::MatrixConstIter() noexcept:
  matrix_(nullptr),
  index_(0)
{}

kondrat::MatrixConstIter::MatrixConstIter(const MatrixIter & other) noexcept:
  matrix_(other.matrix_),
  index_(other.index_)
{}

kondrat::MatrixConstIter::MatrixConstIter(const Matrix * matrix, size_t index) noexcept:
  matrix_(matrix),
  index_(index)
{}

const kondrat::ll & kondrat::MatrixConstIter::operator*() const noexcept
{
  return matrix_->data_[index_];
}

const kondrat::ll * kondrat::MatrixConstIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixConstIter & kondrat::MatrixConstIter::operator++() noexcept
{
  ++index_;
  return *this;
}

kondrat::MatrixConstIter kondrat::MatrixConstIter::operator++(int) noexcept
{
  MatrixConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixConstIter::operator==(const MatrixConstIter & other) const noexcept
{
  return matrix_ == other.matrix_ && index_ == other.index_;
}

bool kondrat::MatrixConstIter::operator!=(const MatrixConstIter & other) const noexcept
{
  return !(*this == other);
}

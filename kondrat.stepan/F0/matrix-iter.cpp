#include "matrix-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixIter::MatrixIter() noexcept:
  matrix_(nullptr),
  index_(0)
{}

kondrat::MatrixIter::MatrixIter(Matrix * matrix, size_t index) noexcept:
  matrix_(matrix),
  index_(index)
{}

kondrat::ll & kondrat::MatrixIter::operator*() const noexcept
{
  return matrix_->data_[index_];
}

kondrat::ll * kondrat::MatrixIter::operator->() const noexcept
{
  return std::addressof(operator*());
}

kondrat::MatrixIter & kondrat::MatrixIter::operator++() noexcept
{
  ++index_;
  return *this;
}

kondrat::MatrixIter kondrat::MatrixIter::operator++(int) noexcept
{
  MatrixIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixIter::operator==(const MatrixIter & other) const noexcept
{
  return matrix_ == other.matrix_ && index_ == other.index_;
}

bool kondrat::MatrixIter::operator!=(const MatrixIter & other) const noexcept
{
  return !(*this == other);
}

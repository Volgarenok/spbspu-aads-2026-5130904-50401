#include "matrix.hpp"
#include <limits>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace kondrat
{
  static size_t getMatrixSize(size_t rows, size_t cols);
  static size_t addSizes(size_t lhs, size_t rhs);
  static ll addValues(ll lhs, ll rhs);
  static ll subValues(ll lhs, ll rhs);
  static ll mulValues(ll lhs, ll rhs);
}

size_t kondrat::getMatrixSize(size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    throw std::logic_error("invalid matrix size");
  }
  if (rows > std::numeric_limits< size_t >::max() / cols)
  {
    throw std::overflow_error("matrix size overflow");
  }
  return rows * cols;
}

size_t kondrat::addSizes(size_t lhs, size_t rhs)
{
  if (lhs > std::numeric_limits< size_t >::max() - rhs)
  {
    throw std::overflow_error("matrix size overflow");
  }
  return lhs + rhs;
}

kondrat::ll kondrat::addValues(ll lhs, ll rhs)
{
  if ((rhs > 0 && lhs > std::numeric_limits< ll >::max() - rhs)
      || (rhs < 0 && lhs < std::numeric_limits< ll >::min() - rhs))
  {
    throw std::overflow_error("matrix value overflow");
  }
  return lhs + rhs;
}

kondrat::ll kondrat::subValues(ll lhs, ll rhs)
{
  if ((rhs < 0 && lhs > std::numeric_limits< ll >::max() + rhs)
      || (rhs > 0 && lhs < std::numeric_limits< ll >::min() + rhs))
  {
    throw std::overflow_error("matrix value overflow");
  }
  return lhs - rhs;
}

kondrat::ll kondrat::mulValues(ll lhs, ll rhs)
{
  if (lhs == 0 || rhs == 0)
  {
    return 0;
  }
  if ((lhs == -1 && rhs == std::numeric_limits< ll >::min())
      || (rhs == -1 && lhs == std::numeric_limits< ll >::min()))
  {
    throw std::overflow_error("matrix value overflow");
  }
  if (lhs > 0)
  {
    if ((rhs > 0 && lhs > std::numeric_limits< ll >::max() / rhs)
        || (rhs < 0 && rhs < std::numeric_limits< ll >::min() / lhs))
    {
      throw std::overflow_error("matrix value overflow");
    }
  }
  else if ((rhs > 0 && lhs < std::numeric_limits< ll >::min() / rhs)
      || (rhs < 0 && lhs < std::numeric_limits< ll >::max() / rhs))
  {
    throw std::overflow_error("matrix value overflow");
  }
  return lhs * rhs;
}

kondrat::Matrix::Matrix():
  rows_(0),
  cols_(0),
  data_()
{}

kondrat::Matrix::Matrix(const Matrix & rhs):
  rows_(rhs.rows_),
  cols_(rhs.cols_),
  data_(rhs.data_)
{}

kondrat::Matrix::Matrix(size_t rows, size_t cols):
  rows_(rows),
  cols_(cols),
  data_(getMatrixSize(rows, cols), 0)
{}

kondrat::Matrix & kondrat::Matrix::operator=(const Matrix & rhs)
{
  if (this != std::addressof(rhs))
  {
    Matrix copy(rhs);
    swap(copy);
  }
  return *this;
}

size_t kondrat::Matrix::rows() const noexcept
{
  return rows_;
}

size_t kondrat::Matrix::cols() const noexcept
{
  return cols_;
}

kondrat::ll & kondrat::Matrix::at(size_t row, size_t col)
{
  return data_[index(row, col)];
}

const kondrat::ll & kondrat::Matrix::at(size_t row, size_t col) const
{
  return data_[index(row, col)];
}

kondrat::Matrix::Iterator kondrat::Matrix::begin() noexcept
{
  return Iterator(this, 0);
}

kondrat::Matrix::Iterator kondrat::Matrix::end() noexcept
{
  return Iterator(this, data_.getSize());
}

kondrat::Matrix::ConstIterator kondrat::Matrix::begin() const noexcept
{
  return ConstIterator(this, 0);
}

kondrat::Matrix::ConstIterator kondrat::Matrix::end() const noexcept
{
  return ConstIterator(this, data_.getSize());
}

kondrat::Matrix::ConstIterator kondrat::Matrix::cbegin() const noexcept
{
  return begin();
}

kondrat::Matrix::ConstIterator kondrat::Matrix::cend() const noexcept
{
  return end();
}

kondrat::Matrix::RowIterator kondrat::Matrix::rowBegin(size_t row)
{
  if (row >= rows_)
  {
    throw std::out_of_range("matrix row out of range");
  }
  return RowIterator(this, row, 0);
}

kondrat::Matrix::RowIterator kondrat::Matrix::rowEnd(size_t row)
{
  if (row >= rows_)
  {
    throw std::out_of_range("matrix row out of range");
  }
  return RowIterator(this, row, cols_);
}

kondrat::Matrix::RowConstIterator kondrat::Matrix::rowBegin(size_t row) const
{
  if (row >= rows_)
  {
    throw std::out_of_range("matrix row out of range");
  }
  return RowConstIterator(this, row, 0);
}

kondrat::Matrix::RowConstIterator kondrat::Matrix::rowEnd(size_t row) const
{
  if (row >= rows_)
  {
    throw std::out_of_range("matrix row out of range");
  }
  return RowConstIterator(this, row, cols_);
}

kondrat::Matrix::ColIterator kondrat::Matrix::colBegin(size_t col)
{
  if (col >= cols_)
  {
    throw std::out_of_range("matrix column out of range");
  }
  return ColIterator(this, 0, col);
}

kondrat::Matrix::ColIterator kondrat::Matrix::colEnd(size_t col)
{
  if (col >= cols_)
  {
    throw std::out_of_range("matrix column out of range");
  }
  return ColIterator(this, rows_, col);
}

kondrat::Matrix::ColConstIterator kondrat::Matrix::colBegin(size_t col) const
{
  if (col >= cols_)
  {
    throw std::out_of_range("matrix column out of range");
  }
  return ColConstIterator(this, 0, col);
}

kondrat::Matrix::ColConstIterator kondrat::Matrix::colEnd(size_t col) const
{
  if (col >= cols_)
  {
    throw std::out_of_range("matrix column out of range");
  }
  return ColConstIterator(this, rows_, col);
}

kondrat::Matrix::DiagIterator kondrat::Matrix::mainDiagBegin() noexcept
{
  return DiagIterator(this, 0, false);
}

kondrat::Matrix::DiagIterator kondrat::Matrix::mainDiagEnd() noexcept
{
  const size_t size = rows_ < cols_ ? rows_ : cols_;
  return DiagIterator(this, size, false);
}

kondrat::Matrix::DiagConstIterator kondrat::Matrix::mainDiagBegin() const noexcept
{
  return DiagConstIterator(this, 0, false);
}

kondrat::Matrix::DiagConstIterator kondrat::Matrix::mainDiagEnd() const noexcept
{
  const size_t size = rows_ < cols_ ? rows_ : cols_;
  return DiagConstIterator(this, size, false);
}

kondrat::Matrix::DiagIterator kondrat::Matrix::sideDiagBegin() noexcept
{
  return DiagIterator(this, 0, true);
}

kondrat::Matrix::DiagIterator kondrat::Matrix::sideDiagEnd() noexcept
{
  const size_t size = rows_ < cols_ ? rows_ : cols_;
  return DiagIterator(this, size, true);
}

kondrat::Matrix::DiagConstIterator kondrat::Matrix::sideDiagBegin() const noexcept
{
  return DiagConstIterator(this, 0, true);
}

kondrat::Matrix::DiagConstIterator kondrat::Matrix::sideDiagEnd() const noexcept
{
  const size_t size = rows_ < cols_ ? rows_ : cols_;
  return DiagConstIterator(this, size, true);
}

kondrat::Matrix kondrat::Matrix::operator+(const Matrix & rhs) const
{
  if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
  {
    throw std::logic_error("different matrix sizes");
  }

  Matrix result(rows_, cols_);
  ConstIterator lhsIt = cbegin();
  ConstIterator rhsIt = rhs.cbegin();
  for (Iterator resultIt = result.begin(); resultIt != result.end(); ++resultIt)
  {
    *resultIt = addValues(*lhsIt, *rhsIt);
    ++lhsIt;
    ++rhsIt;
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::operator-(const Matrix & rhs) const
{
  if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
  {
    throw std::logic_error("different matrix sizes");
  }

  Matrix result(rows_, cols_);
  ConstIterator lhsIt = cbegin();
  ConstIterator rhsIt = rhs.cbegin();
  for (Iterator resultIt = result.begin(); resultIt != result.end(); ++resultIt)
  {
    *resultIt = subValues(*lhsIt, *rhsIt);
    ++lhsIt;
    ++rhsIt;
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::operator*(const Matrix & rhs) const
{
  if (cols_ != rhs.rows_)
  {
    throw std::logic_error("incompatible matrix sizes");
  }

  Matrix result(rows_, rhs.cols_);
  for (size_t i = 0; i < rows_; ++i)
  {
    RowIterator resultIt = result.rowBegin(i);
    for (size_t j = 0; j < rhs.cols_; ++j)
    {
      ll value = 0;
      RowConstIterator rowIt = rowBegin(i);
      const RowConstIterator rowEndIt = rowEnd(i);
      ColConstIterator colIt = rhs.colBegin(j);
      while (rowIt != rowEndIt)
      {
        value = addValues(value, mulValues(*rowIt, *colIt));
        ++rowIt;
        ++colIt;
      }
      *resultIt = value;
      ++resultIt;
    }
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::operator*(ll number) const
{
  Matrix result(rows_, cols_);
  ConstIterator sourceIt = cbegin();
  for (Iterator resultIt = result.begin(); resultIt != result.end(); ++resultIt)
  {
    *resultIt = mulValues(*sourceIt, number);
    ++sourceIt;
  }
  return result;
}

bool kondrat::Matrix::operator==(const Matrix & rhs) const noexcept
{
  if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
  {
    return false;
  }

  ConstIterator lhsIt = cbegin();
  ConstIterator rhsIt = rhs.cbegin();
  while (lhsIt != cend())
  {
    if (*lhsIt != *rhsIt)
    {
      return false;
    }
    ++lhsIt;
    ++rhsIt;
  }
  return true;
}

bool kondrat::Matrix::operator!=(const Matrix & rhs) const noexcept
{
  return !(*this == rhs);
}

kondrat::Matrix kondrat::Matrix::getPower(size_t degree) const
{
  if (rows_ != cols_)
  {
    throw std::logic_error("matrix is not square");
  }

  Matrix result(rows_, cols_);
  for (DiagIterator it = result.mainDiagBegin(); it != result.mainDiagEnd(); ++it)
  {
    *it = 1;
  }

  Matrix factor(*this);
  while (degree != 0)
  {
    if (degree % 2 != 0)
    {
      result = result * factor;
    }
    degree /= 2;
    if (degree != 0)
    {
      factor = factor * factor;
    }
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::getInsertedRow(size_t targetRow, const Matrix & source, size_t sourceRow) const
{
  if (targetRow > rows_ || sourceRow >= source.rows_ || cols_ != source.cols_)
  {
    throw std::logic_error("invalid row insertion");
  }

  Matrix result(addSizes(rows_, 1), cols_);
  for (size_t i = 0; i < result.rows_; ++i)
  {
    RowIterator resultIt = result.rowBegin(i);
    const RowIterator resultEnd = result.rowEnd(i);

    if (i == targetRow)
    {
      RowConstIterator sourceIt = source.rowBegin(sourceRow);
      while (resultIt != resultEnd)
      {
        *resultIt = *sourceIt;
        ++resultIt;
        ++sourceIt;
      }
      continue;
    }

    const size_t currentRow = i < targetRow ? i : i - 1;
    RowConstIterator sourceIt = rowBegin(currentRow);
    while (resultIt != resultEnd)
    {
      *resultIt = *sourceIt;
      ++resultIt;
      ++sourceIt;
    }
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::getInsertedCol(size_t targetCol, const Matrix & source, size_t sourceCol) const
{
  if (targetCol > cols_ || sourceCol >= source.cols_ || rows_ != source.rows_)
  {
    throw std::logic_error("invalid column insertion");
  }

  Matrix result(rows_, addSizes(cols_, 1));
  for (size_t j = 0; j < result.cols_; ++j)
  {
    ColIterator resultIt = result.colBegin(j);
    const ColIterator resultEnd = result.colEnd(j);

    if (j == targetCol)
    {
      ColConstIterator sourceIt = source.colBegin(sourceCol);
      while (resultIt != resultEnd)
      {
        *resultIt = *sourceIt;
        ++resultIt;
        ++sourceIt;
      }
      continue;
    }

    const size_t currentCol = j < targetCol ? j : j - 1;
    ColConstIterator sourceIt = colBegin(currentCol);
    while (resultIt != resultEnd)
    {
      *resultIt = *sourceIt;
      ++resultIt;
      ++sourceIt;
    }
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::getInserted(const Matrix & source, size_t startRow, size_t startCol) const
{
  if (startRow > rows_ || startCol > cols_
      || source.rows_ > rows_ - startRow || source.cols_ > cols_ - startCol)
  {
    throw std::logic_error("matrix does not fit");
  }

  Matrix result(*this);
  for (size_t i = 0; i < source.rows_; ++i)
  {
    RowIterator resultIt = result.rowBegin(startRow + i);
    for (size_t j = 0; j < startCol; ++j)
    {
      ++resultIt;
    }

    RowConstIterator sourceIt = source.rowBegin(i);
    const RowConstIterator sourceEnd = source.rowEnd(i);
    while (sourceIt != sourceEnd)
    {
      *resultIt = *sourceIt;
      ++resultIt;
      ++sourceIt;
    }
  }
  return result;
}

kondrat::Matrix kondrat::Matrix::getMinor(size_t row, size_t col) const
{
  if (rows_ < 2 || cols_ < 2 || row >= rows_ || col >= cols_)
  {
    throw std::logic_error("invalid minor");
  }

  Matrix result(rows_ - 1, cols_ - 1);
  size_t resultRow = 0;
  for (size_t i = 0; i < rows_; ++i)
  {
    if (i == row)
    {
      continue;
    }

    size_t resultCol = 0;
    for (size_t j = 0; j < cols_; ++j)
    {
      if (j != col)
      {
        result.at(resultRow, resultCol) = at(i, j);
        ++resultCol;
      }
    }
    ++resultRow;
  }
  return result;
}

kondrat::ll kondrat::Matrix::getDeterminant() const
{
  if (rows_ == 0 || rows_ != cols_)
  {
    throw std::logic_error("matrix is not square");
  }
  if (rows_ == 1)
  {
    return at(0, 0);
  }

  kondrat::Vector< ll > values(rows_ * cols_, 0);
  for (size_t i = 0; i < rows_; ++i)
  {
    for (size_t j = 0; j < cols_; ++j)
    {
      values[i * cols_ + j] = at(i, j);
    }
  }

  ll previousPivot = 1;
  int sign = 1;
  for (size_t k = 0; k + 1 < rows_; ++k)
  {
    size_t pivotRow = k;
    while (pivotRow < rows_ && values[pivotRow * cols_ + k] == 0)
    {
      ++pivotRow;
    }
    if (pivotRow == rows_)
    {
      return 0;
    }
    if (pivotRow != k)
    {
      for (size_t j = 0; j < cols_; ++j)
      {
        std::swap(values[k * cols_ + j], values[pivotRow * cols_ + j]);
      }
      sign = -sign;
    }

    const ll pivot = values[k * cols_ + k];
    for (size_t i = k + 1; i < rows_; ++i)
    {
      for (size_t j = k + 1; j < cols_; ++j)
      {
        ll value = mulValues(values[i * cols_ + j], pivot);
        const ll product = mulValues(values[i * cols_ + k], values[k * cols_ + j]);
        value = subValues(value, product);
        value /= previousPivot;
        values[i * cols_ + j] = value;
      }
      values[i * cols_ + k] = 0;
    }
    previousPivot = pivot;
  }

  const ll determinant = values[(rows_ - 1) * cols_ + rows_ - 1];
  return mulValues(determinant, sign);
}

size_t kondrat::Matrix::getRank() const
{
  kondrat::Vector< ll > values(data_);
  size_t rank = 0;
  ll previousPivot = 1;
  for (size_t col = 0; col < cols_ && rank < rows_; ++col)
  {
    size_t pivotRow = rank;
    while (pivotRow < rows_ && values[pivotRow * cols_ + col] == 0)
    {
      ++pivotRow;
    }
    if (pivotRow == rows_)
    {
      continue;
    }

    for (size_t j = col; j < cols_; ++j)
    {
      std::swap(values[rank * cols_ + j], values[pivotRow * cols_ + j]);
    }

    const ll pivot = values[rank * cols_ + col];
    for (size_t i = rank + 1; i < rows_; ++i)
    {
      for (size_t j = col + 1; j < cols_; ++j)
      {
        ll value = mulValues(values[i * cols_ + j], pivot);
        const ll product = mulValues(values[i * cols_ + col], values[rank * cols_ + j]);
        value = subValues(value, product);
        value /= previousPivot;
        values[i * cols_ + j] = value;
      }
      values[i * cols_ + col] = 0;
    }
    previousPivot = pivot;
    ++rank;
  }
  return rank;
}

void kondrat::Matrix::swap(Matrix & other) noexcept
{
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  data_.swap(other.data_);
}

kondrat::Matrix kondrat::concatRows(const kondrat::Vector< Matrix > & matrices)
{
  if (matrices.isEmpty())
  {
    throw std::logic_error("no matrices");
  }

  size_t rows = 0;
  size_t cols = 0;
  for (size_t i = 0; i < matrices.getSize(); ++i)
  {
    rows = matrices[i].rows() > rows ? matrices[i].rows() : rows;
    cols = addSizes(cols, matrices[i].cols());
  }

  Matrix result(rows, cols);
  size_t colOffset = 0;
  for (size_t k = 0; k < matrices.getSize(); ++k)
  {
    for (size_t i = 0; i < matrices[k].rows(); ++i)
    {
      Matrix::RowIterator resultIt = result.rowBegin(i);
      for (size_t j = 0; j < colOffset; ++j)
      {
        ++resultIt;
      }

      Matrix::RowConstIterator sourceIt = matrices[k].rowBegin(i);
      const Matrix::RowConstIterator sourceEnd = matrices[k].rowEnd(i);
      while (sourceIt != sourceEnd)
      {
        *resultIt = *sourceIt;
        ++resultIt;
        ++sourceIt;
      }
    }
    colOffset += matrices[k].cols();
  }
  return result;
}

kondrat::Matrix kondrat::concatCols(const kondrat::Vector< Matrix > & matrices)
{
  if (matrices.isEmpty())
  {
    throw std::logic_error("no matrices");
  }

  size_t rows = 0;
  size_t cols = 0;
  for (size_t i = 0; i < matrices.getSize(); ++i)
  {
    rows = addSizes(rows, matrices[i].rows());
    cols = matrices[i].cols() > cols ? matrices[i].cols() : cols;
  }

  Matrix result(rows, cols);
  size_t rowOffset = 0;
  for (size_t k = 0; k < matrices.getSize(); ++k)
  {
    for (size_t i = 0; i < matrices[k].rows(); ++i)
    {
      Matrix::RowIterator resultIt = result.rowBegin(rowOffset + i);
      Matrix::RowConstIterator sourceIt = matrices[k].rowBegin(i);
      const Matrix::RowConstIterator sourceEnd = matrices[k].rowEnd(i);
      while (sourceIt != sourceEnd)
      {
        *resultIt = *sourceIt;
        ++resultIt;
        ++sourceIt;
      }
    }
    rowOffset += matrices[k].rows();
  }
  return result;
}

kondrat::Matrix kondrat::concatMainDiag(const kondrat::Vector< Matrix > & matrices)
{
  if (matrices.isEmpty())
  {
    throw std::logic_error("no matrices");
  }

  size_t rows = 0;
  size_t cols = 0;
  for (size_t i = 0; i < matrices.getSize(); ++i)
  {
    rows = addSizes(rows, matrices[i].rows());
    cols = addSizes(cols, matrices[i].cols());
  }

  Matrix result(rows, cols);
  size_t rowOffset = 0;
  size_t colOffset = 0;
  for (size_t k = 0; k < matrices.getSize(); ++k)
  {
    result = result.getInserted(matrices[k], rowOffset, colOffset);
    rowOffset += matrices[k].rows();
    colOffset += matrices[k].cols();
  }
  return result;
}

kondrat::Matrix kondrat::concatSideDiag(const kondrat::Vector< Matrix > & matrices)
{
  if (matrices.isEmpty())
  {
    throw std::logic_error("no matrices");
  }

  size_t rows = 0;
  size_t cols = 0;
  for (size_t i = 0; i < matrices.getSize(); ++i)
  {
    rows = addSizes(rows, matrices[i].rows());
    cols = addSizes(cols, matrices[i].cols());
  }

  Matrix result(rows, cols);
  size_t rowOffset = 0;
  size_t colOffset = cols;
  for (size_t k = 0; k < matrices.getSize(); ++k)
  {
    colOffset -= matrices[k].cols();
    result = result.getInserted(matrices[k], rowOffset, colOffset);
    rowOffset += matrices[k].rows();
  }
  return result;
}

size_t kondrat::Matrix::index(size_t row, size_t col) const
{
  if (row >= rows_ || col >= cols_)
  {
    throw std::out_of_range("matrix index out of range");
  }
  return row * cols_ + col;
}

kondrat::Matrix kondrat::operator*(ll number, const Matrix & matrix)
{
  return matrix * number;
}

std::ostream & kondrat::operator<<(std::ostream & out, const Matrix & matrix)
{
  for (size_t i = 0; i < matrix.rows(); ++i)
  {
    Matrix::RowConstIterator it = matrix.rowBegin(i);
    const Matrix::RowConstIterator end = matrix.rowEnd(i);
    while (it != end)
    {
      if (it != matrix.rowBegin(i))
      {
        out << ' ';
      }
      out << *it;
      ++it;
    }
    if (i + 1 < matrix.rows())
    {
      out << '\n';
    }
  }
  return out;
}

#include "matrix.hpp"

#include <cmath>
#include <iomanip>
#include <stdexcept>

lukashevich::Matrix::Matrix():
  rows_(0),
  cols_(0),
  data_()
{}

lukashevich::Matrix::Matrix(size_t rows, size_t cols):
  rows_(rows),
  cols_(cols),
  data_(rows * cols, 0.0)
{
  if ((rows == 0) || (cols == 0)) {
    throw std::invalid_argument("bad matrix size");
  }
}

lukashevich::Matrix::Matrix(size_t rows, size_t cols, const Vector< double > & values):
  rows_(rows),
  cols_(cols),
  data_(values)
{
  if ((rows == 0) || (cols == 0)) {
    throw std::invalid_argument("bad matrix size");
  }
  if (values.getSize() != rows * cols) {
    throw std::invalid_argument("bad matrix data size");
  }
}

double & lukashevich::Matrix::operator()(size_t row, size_t col)
{
  return data_[getIndex(row, col)];
}

const double & lukashevich::Matrix::operator()(size_t row, size_t col) const
{
  return data_[getIndex(row, col)];
}

bool lukashevich::Matrix::operator==(const Matrix & rhs) const
{
  if ((rows_ != rhs.rows_) || (cols_ != rhs.cols_)) {
    return false;
  }

  for (size_t i = 0; i < data_.getSize(); ++i) {
    if (data_[i] != rhs.data_[i]) {
      return false;
    }
  }

  return true;
}

bool lukashevich::Matrix::operator!=(const Matrix & rhs) const
{
  return !(*this == rhs);
}

lukashevich::Matrix lukashevich::Matrix::operator+(const Matrix & rhs) const
{
  if ((rows_ != rhs.rows_) || (cols_ != rhs.cols_)) {
    throw std::invalid_argument("bad matrix sizes");
  }

  Matrix result(rows_, cols_);
  for (size_t i = 0; i < data_.getSize(); ++i) {
    result.data_[i] = data_[i] + rhs.data_[i];
  }

  return result;
}

lukashevich::Matrix lukashevich::Matrix::operator-(const Matrix & rhs) const
{
  if ((rows_ != rhs.rows_) || (cols_ != rhs.cols_)) {
    throw std::invalid_argument("bad matrix sizes");
  }

  Matrix result(rows_, cols_);
  for (size_t i = 0; i < data_.getSize(); ++i) {
    result.data_[i] = data_[i] - rhs.data_[i];
  }

  return result;
}

lukashevich::Matrix lukashevich::Matrix::operator*(const Matrix & rhs) const
{
  if (cols_ != rhs.rows_) {
    throw std::invalid_argument("bad matrix sizes");
  }

  Matrix result(rows_, rhs.cols_);
  for (size_t row = 0; row < rows_; ++row) {
    for (size_t col = 0; col < rhs.cols_; ++col) {
      double value = 0.0;
      for (size_t index = 0; index < cols_; ++index) {
        value += (*this)(row, index) * rhs(index, col);
      }
      result(row, col) = value;
    }
  }

  return result;
}

lukashevich::Matrix lukashevich::Matrix::operator*(double value) const
{
  Matrix result(rows_, cols_);
  for (size_t i = 0; i < data_.getSize(); ++i) {
    result.data_[i] = data_[i] * value;
  }

  return result;
}

size_t lukashevich::Matrix::getRows() const noexcept
{
  return rows_;
}

size_t lukashevich::Matrix::getCols() const noexcept
{
  return cols_;
}

size_t lukashevich::Matrix::getSize() const noexcept
{
  return data_.getSize();
}

lukashevich::Matrix lukashevich::Matrix::getTransposed() const
{
  Matrix result(cols_, rows_);

  for (size_t row = 0; row < rows_; ++row) {
    for (size_t col = 0; col < cols_; ++col) {
      result(col, row) = (*this)(row, col);
    }
  }

  return result;
}

double lukashevich::Matrix::getDeterminant() const
{
  if (rows_ != cols_) {
    throw std::invalid_argument("matrix is not square");
  }

  Matrix temp(*this);
  double result = 1.0;

  for (size_t col = 0; col < cols_; ++col) {
    size_t pivot = col;
    double best = std::fabs(temp(col, col));

    for (size_t row = col + 1; row < rows_; ++row) {
      const double current = std::fabs(temp(row, col));
      if (current > best) {
        best = current;
        pivot = row;
      }
    }

    if (temp(pivot, col) == 0.0) {
      return 0.0;
    }

    if (pivot != col) {
      temp.swapRows(pivot, col);
      result = -result;
    }

    const double pivotValue = temp(col, col);
    for (size_t row = col + 1; row < rows_; ++row) {
      const double factor = temp(row, col) / pivotValue;
      for (size_t nextCol = col; nextCol < cols_; ++nextCol) {
        temp(row, nextCol) -= factor * temp(col, nextCol);
      }
    }
  }

  for (size_t i = 0; i < rows_; ++i) {
    result *= temp(i, i);
  }

  if (result == 0.0) {
    return 0.0;
  }
  return result;
}

size_t lukashevich::Matrix::getRank() const
{
  Matrix temp(*this);
  size_t row = 0;

  for (size_t col = 0; (col < cols_) && (row < rows_); ++col) {
    size_t pivot = row;
    double best = std::fabs(temp(row, col));

    for (size_t nextRow = row + 1; nextRow < rows_; ++nextRow) {
      const double current = std::fabs(temp(nextRow, col));
      if (current > best) {
        best = current;
        pivot = nextRow;
      }
    }

    if (temp(pivot, col) == 0.0) {
      continue;
    }

    if (pivot != row) {
      temp.swapRows(pivot, row);
    }

    const double pivotValue = temp(row, col);
    for (size_t nextRow = row + 1; nextRow < rows_; ++nextRow) {
      const double factor = temp(nextRow, col) / pivotValue;
      for (size_t nextCol = col; nextCol < cols_; ++nextCol) {
        temp(nextRow, nextCol) -= factor * temp(row, nextCol);
      }
    }

    ++row;
  }

  return row;
}

lukashevich::Matrix lukashevich::Matrix::getInversed() const
{
  if (rows_ != cols_) {
    throw std::invalid_argument("matrix is not square");
  }

  const size_t n = rows_;
  Matrix temp(n, n * 2);

  for (size_t row = 0; row < n; ++row) {
    for (size_t col = 0; col < n; ++col) {
      temp(row, col) = (*this)(row, col);
    }
    temp(row, n + row) = 1.0;
  }

  for (size_t col = 0; col < n; ++col) {
    size_t pivot = col;
    double best = std::fabs(temp(col, col));

    for (size_t row = col + 1; row < n; ++row) {
      const double current = std::fabs(temp(row, col));
      if (current > best) {
        best = current;
        pivot = row;
      }
    }

    if (temp(pivot, col) == 0.0) {
      throw std::invalid_argument("singular matrix");
    }

    if (pivot != col) {
      temp.swapRows(pivot, col);
    }

    const double pivotValue = temp(col, col);
    for (size_t nextCol = 0; nextCol < n * 2; ++nextCol) {
      temp(col, nextCol) /= pivotValue;
    }

    for (size_t row = 0; row < n; ++row) {
      if (row != col) {
        const double factor = temp(row, col);
        for (size_t nextCol = 0; nextCol < n * 2; ++nextCol) {
          temp(row, nextCol) -= factor * temp(col, nextCol);
        }
      }
    }
  }

  Matrix result(n, n);
  for (size_t row = 0; row < n; ++row) {
    for (size_t col = 0; col < n; ++col) {
      result(row, col) = temp(row, n + col);
    }
  }

  return result;
}

void lukashevich::Matrix::swapRows(size_t first, size_t second)
{
  if ((first >= rows_) || (second >= rows_)) {
    throw std::out_of_range("bad row index");
  }

  if (first == second) {
    return;
  }

  for (size_t col = 0; col < cols_; ++col) {
    std::swap((*this)(first, col), (*this)(second, col));
  }
}

void lukashevich::Matrix::appendRows(const Matrix & rhs)
{
  if (cols_ != rhs.cols_) {
    throw std::invalid_argument("bad matrix sizes");
  }

  Matrix result(rows_ + rhs.rows_, cols_);
  for (size_t row = 0; row < rows_; ++row) {
    for (size_t col = 0; col < cols_; ++col) {
      result(row, col) = (*this)(row, col);
    }
  }
  for (size_t row = 0; row < rhs.rows_; ++row) {
    for (size_t col = 0; col < rhs.cols_; ++col) {
      result(rows_ + row, col) = rhs(row, col);
    }
  }

  *this = result;
}

void lukashevich::Matrix::appendCols(const Matrix & rhs)
{
  if (rows_ != rhs.rows_) {
    throw std::invalid_argument("bad matrix sizes");
  }

  Matrix result(rows_, cols_ + rhs.cols_);
  for (size_t row = 0; row < rows_; ++row) {
    for (size_t col = 0; col < cols_; ++col) {
      result(row, col) = (*this)(row, col);
    }
    for (size_t col = 0; col < rhs.cols_; ++col) {
      result(row, cols_ + col) = rhs(row, col);
    }
  }

  *this = result;
}

lukashevich::MatrixIterator lukashevich::Matrix::begin() noexcept
{
  return MatrixIterator(this, 0);
}

lukashevich::MatrixIterator lukashevich::Matrix::end() noexcept
{
  return MatrixIterator(this, data_.getSize());
}

lukashevich::MatrixConstIterator lukashevich::Matrix::begin() const noexcept
{
  return MatrixConstIterator(this, 0);
}

lukashevich::MatrixConstIterator lukashevich::Matrix::end() const noexcept
{
  return MatrixConstIterator(this, data_.getSize());
}

lukashevich::MatrixConstIterator lukashevich::Matrix::cbegin() const noexcept
{
  return begin();
}

lukashevich::MatrixConstIterator lukashevich::Matrix::cend() const noexcept
{
  return end();
}

lukashevich::MatrixRowIterator lukashevich::Matrix::rowBegin(size_t row)
{
  if (row >= rows_) {
    throw std::out_of_range("bad row index");
  }
  return MatrixRowIterator(this, row, 0);
}

lukashevich::MatrixRowIterator lukashevich::Matrix::rowEnd(size_t row)
{
  if (row >= rows_) {
    throw std::out_of_range("bad row index");
  }
  return MatrixRowIterator(this, row, cols_);
}

lukashevich::MatrixRowConstIterator lukashevich::Matrix::rowBegin(size_t row) const
{
  if (row >= rows_) {
    throw std::out_of_range("bad row index");
  }
  return MatrixRowConstIterator(this, row, 0);
}

lukashevich::MatrixRowConstIterator lukashevich::Matrix::rowEnd(size_t row) const
{
  if (row >= rows_) {
    throw std::out_of_range("bad row index");
  }
  return MatrixRowConstIterator(this, row, cols_);
}

lukashevich::MatrixColIterator lukashevich::Matrix::colBegin(size_t col)
{
  if (col >= cols_) {
    throw std::out_of_range("bad col index");
  }
  return MatrixColIterator(this, 0, col);
}

lukashevich::MatrixColIterator lukashevich::Matrix::colEnd(size_t col)
{
  if (col >= cols_) {
    throw std::out_of_range("bad col index");
  }
  return MatrixColIterator(this, rows_, col);
}

lukashevich::MatrixColConstIterator lukashevich::Matrix::colBegin(size_t col) const
{
  if (col >= cols_) {
    throw std::out_of_range("bad col index");
  }
  return MatrixColConstIterator(this, 0, col);
}

lukashevich::MatrixColConstIterator lukashevich::Matrix::colEnd(size_t col) const
{
  if (col >= cols_) {
    throw std::out_of_range("bad col index");
  }
  return MatrixColConstIterator(this, rows_, col);
}

lukashevich::MatrixDiagIterator lukashevich::Matrix::diagBegin(bool isMainDiagonal)
{
  return MatrixDiagIterator(this, 0, isMainDiagonal);
}

lukashevich::MatrixDiagIterator lukashevich::Matrix::diagEnd(bool isMainDiagonal)
{
  return MatrixDiagIterator(this, getMinSize(rows_, cols_), isMainDiagonal);
}

lukashevich::MatrixDiagConstIterator lukashevich::Matrix::diagBegin(
    bool isMainDiagonal) const
{
  return MatrixDiagConstIterator(this, 0, isMainDiagonal);
}

lukashevich::MatrixDiagConstIterator lukashevich::Matrix::diagEnd(
    bool isMainDiagonal) const
{
  return MatrixDiagConstIterator(this, getMinSize(rows_, cols_), isMainDiagonal);
}

size_t lukashevich::Matrix::getIndex(size_t row, size_t col) const
{
  if ((row >= rows_) || (col >= cols_)) {
    throw std::out_of_range("bad matrix index");
  }

  return (row * cols_) + col;
}

size_t lukashevich::Matrix::getMinSize(size_t first, size_t second) noexcept
{
  if (first < second) {
    return first;
  }
  return second;
}

lukashevich::Matrix lukashevich::operator*(double value, const Matrix & matrix)
{
  return matrix * value;
}

lukashevich::Matrix lukashevich::mergeVertical(const Matrix & first,
    const Matrix & second,
    double fillValue)
{
  size_t cols = first.getCols();
  if (second.getCols() > cols) {
    cols = second.getCols();
  }

  Matrix result(first.getRows() + second.getRows(), cols);
  for (size_t row = 0; row < result.getRows(); ++row) {
    for (size_t col = 0; col < result.getCols(); ++col) {
      result(row, col) = fillValue;
    }
  }
  for (size_t row = 0; row < first.getRows(); ++row) {
    for (size_t col = 0; col < first.getCols(); ++col) {
      result(row, col) = first(row, col);
    }
  }
  for (size_t row = 0; row < second.getRows(); ++row) {
    for (size_t col = 0; col < second.getCols(); ++col) {
      result(first.getRows() + row, col) = second(row, col);
    }
  }

  return result;
}

lukashevich::Matrix lukashevich::mergeHorizontal(const Matrix & first,
    const Matrix & second,
    double fillValue)
{
  size_t rows = first.getRows();
  if (second.getRows() > rows) {
    rows = second.getRows();
  }

  Matrix result(rows, first.getCols() + second.getCols());
  for (size_t row = 0; row < result.getRows(); ++row) {
    for (size_t col = 0; col < result.getCols(); ++col) {
      result(row, col) = fillValue;
    }
  }
  for (size_t row = 0; row < first.getRows(); ++row) {
    for (size_t col = 0; col < first.getCols(); ++col) {
      result(row, col) = first(row, col);
    }
  }
  for (size_t row = 0; row < second.getRows(); ++row) {
    for (size_t col = 0; col < second.getCols(); ++col) {
      result(row, first.getCols() + col) = second(row, col);
    }
  }

  return result;
}

lukashevich::Matrix lukashevich::tileHorizontal(const Matrix & matrix, size_t count)
{
  if (count == 0) {
    throw std::invalid_argument("bad tile count");
  }

  Matrix result(matrix.getRows(), matrix.getCols() * count);
  for (size_t part = 0; part < count; ++part) {
    for (size_t row = 0; row < matrix.getRows(); ++row) {
      for (size_t col = 0; col < matrix.getCols(); ++col) {
        result(row, (part * matrix.getCols()) + col) = matrix(row, col);
      }
    }
  }

  return result;
}

lukashevich::Matrix lukashevich::tileVertical(const Matrix & matrix, size_t count)
{
  if (count == 0) {
    throw std::invalid_argument("bad tile count");
  }

  Matrix result(matrix.getRows() * count, matrix.getCols());
  for (size_t part = 0; part < count; ++part) {
    for (size_t row = 0; row < matrix.getRows(); ++row) {
      for (size_t col = 0; col < matrix.getCols(); ++col) {
        result((part * matrix.getRows()) + row, col) = matrix(row, col);
      }
    }
  }

  return result;
}

lukashevich::Matrix lukashevich::makeFilledMatrix(size_t rows, size_t cols, double value)
{
  Matrix result(rows, cols);
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      result(row, col) = value;
    }
  }

  return result;
}

void lukashevich::printMatrix(std::ostream & out,
    const std::string & name,
    const Matrix & matrix)
{
  out << name << " [" << matrix.getRows() << "x" << matrix.getCols() << "]:\n";

  for (size_t row = 0; row < matrix.getRows(); ++row) {
    for (size_t col = 0; col < matrix.getCols(); ++col) {
      if (col != 0) {
        out << ' ';
      }
      printDouble(out, matrix(row, col));
    }
    out << '\n';
  }
}

void lukashevich::printDouble(std::ostream & out, double value)
{
  if (value == 0.0) {
    value = 0.0;
  }

  out << std::fixed << std::setprecision(2) << value;
}

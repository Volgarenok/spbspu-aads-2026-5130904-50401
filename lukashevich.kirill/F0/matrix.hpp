#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <iosfwd>
#include <string>

#include "vector/vector.hpp"
#include "matrix-col-const-iter.hpp"
#include "matrix-col-iter.hpp"
#include "matrix-const-iter.hpp"
#include "matrix-iter.hpp"
#include "matrix-row-const-iter.hpp"
#include "matrix-row-iter.hpp"
#include "matrix-diag-const-iter.hpp"
#include "matrix-diag-iter.hpp"

namespace lukashevich {
  class Matrix
  {
  public:
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, const Vector< double > & values);

    double & operator()(size_t row, size_t col);
    const double & operator()(size_t row, size_t col) const;

    bool operator==(const Matrix & rhs) const;
    bool operator!=(const Matrix & rhs) const;
    Matrix operator+(const Matrix & rhs) const;
    Matrix operator-(const Matrix & rhs) const;
    Matrix operator*(const Matrix & rhs) const;
    Matrix operator*(double value) const;

    size_t getRows() const noexcept;
    size_t getCols() const noexcept;
    size_t getSize() const noexcept;

    Matrix getTransposed() const;
    double getDeterminant() const;
    size_t getRank() const;
    Matrix getInversed() const;

    void swapRows(size_t first, size_t second);
    void appendRows(const Matrix & rhs);
    void appendCols(const Matrix & rhs);

    MatrixIterator begin() noexcept;
    MatrixIterator end() noexcept;
    MatrixConstIterator begin() const noexcept;
    MatrixConstIterator end() const noexcept;
    MatrixConstIterator cbegin() const noexcept;
    MatrixConstIterator cend() const noexcept;

    MatrixRowIterator rowBegin(size_t row);
    MatrixRowIterator rowEnd(size_t row);
    MatrixRowConstIterator rowBegin(size_t row) const;
    MatrixRowConstIterator rowEnd(size_t row) const;

    MatrixColIterator colBegin(size_t col);
    MatrixColIterator colEnd(size_t col);
    MatrixColConstIterator colBegin(size_t col) const;
    MatrixColConstIterator colEnd(size_t col) const;

    MatrixDiagIterator diagBegin(bool isMainDiagonal);
    MatrixDiagIterator diagEnd(bool isMainDiagonal);
    MatrixDiagConstIterator diagBegin(bool isMainDiagonal) const;
    MatrixDiagConstIterator diagEnd(bool isMainDiagonal) const;

  private:
    size_t getIndex(size_t row, size_t col) const;
    static size_t getMinSize(size_t first, size_t second) noexcept;

    size_t rows_;
    size_t cols_;
    Vector< double > data_;
  };

  Matrix operator*(double value, const Matrix & matrix);
  Matrix mergeVertical(const Matrix & first, const Matrix & second, double fillValue);
  Matrix mergeHorizontal(const Matrix & first, const Matrix & second, double fillValue);
  Matrix tileHorizontal(const Matrix & matrix, size_t count);
  Matrix tileVertical(const Matrix & matrix, size_t count);
  Matrix makeFilledMatrix(size_t rows, size_t cols, double value);

  void printMatrix(std::ostream & out, const std::string & name, const Matrix & matrix);
  void printDouble(std::ostream & out, double value);
}

#endif

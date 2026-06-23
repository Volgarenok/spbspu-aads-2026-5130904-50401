#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <iosfwd>
#include <vector/top-it-vector.hpp>
#include "matrix-iter.hpp"
#include "matrix-const-iter.hpp"
#include "matrix-row-iter.hpp"
#include "matrix-row-const-iter.hpp"
#include "matrix-col-iter.hpp"
#include "matrix-col-const-iter.hpp"
#include "matrix-diag-iter.hpp"
#include "matrix-diag-const-iter.hpp"

namespace kondrat
{
  using ll = long long;

  class MatrixIter;
  class MatrixConstIter;
  class MatrixRowIter;
  class MatrixRowConstIter;
  class MatrixColIter;
  class MatrixColConstIter;
  class MatrixDiagIter;
  class MatrixDiagConstIter;

  class Matrix
  {
    public:
      using Iterator = MatrixIter;
      using ConstIterator = MatrixConstIter;
      using RowIterator = MatrixRowIter;
      using RowConstIterator = MatrixRowConstIter;
      using ColIterator = MatrixColIter;
      using ColConstIterator = MatrixColConstIter;
      using DiagIterator = MatrixDiagIter;
      using DiagConstIterator = MatrixDiagConstIter;

      Matrix();
      Matrix(const Matrix & rhs);
      Matrix(size_t rows, size_t cols);

      Matrix & operator=(const Matrix & rhs);
      Matrix operator+(const Matrix & rhs) const;
      Matrix operator-(const Matrix & rhs) const;
      Matrix operator*(const Matrix & rhs) const;
      Matrix operator*(ll number) const;
      bool operator==(const Matrix & rhs) const noexcept;
      bool operator!=(const Matrix & rhs) const noexcept;

      size_t rows() const noexcept;
      size_t cols() const noexcept;

      ll & at(size_t row, size_t col);
      const ll & at(size_t row, size_t col) const;

      Iterator begin() noexcept;
      Iterator end() noexcept;
      ConstIterator begin() const noexcept;
      ConstIterator end() const noexcept;
      ConstIterator cbegin() const noexcept;
      ConstIterator cend() const noexcept;

      RowIterator rowBegin(size_t row);
      RowIterator rowEnd(size_t row);
      RowConstIterator rowBegin(size_t row) const;
      RowConstIterator rowEnd(size_t row) const;

      ColIterator colBegin(size_t col);
      ColIterator colEnd(size_t col);
      ColConstIterator colBegin(size_t col) const;
      ColConstIterator colEnd(size_t col) const;

      DiagIterator mainDiagBegin() noexcept;
      DiagIterator mainDiagEnd() noexcept;
      DiagConstIterator mainDiagBegin() const noexcept;
      DiagConstIterator mainDiagEnd() const noexcept;

      DiagIterator sideDiagBegin() noexcept;
      DiagIterator sideDiagEnd() noexcept;
      DiagConstIterator sideDiagBegin() const noexcept;
      DiagConstIterator sideDiagEnd() const noexcept;

      Matrix getPower(size_t degree) const;
      Matrix getInsertedRow(size_t targetRow, const Matrix & source, size_t sourceRow) const;
      Matrix getInsertedCol(size_t targetCol, const Matrix & source, size_t sourceCol) const;
      Matrix getInserted(const Matrix & source, size_t startRow, size_t startCol) const;
      Matrix getMinor(size_t row, size_t col) const;

      ll getDeterminant() const;
      size_t getRank() const;
      void swap(Matrix & other) noexcept;

    private:
      friend class MatrixIter;
      friend class MatrixConstIter;
      friend class MatrixRowIter;
      friend class MatrixRowConstIter;
      friend class MatrixColIter;
      friend class MatrixColConstIter;
      friend class MatrixDiagIter;
      friend class MatrixDiagConstIter;

      size_t rows_;
      size_t cols_;
      kondrat::Vector< ll > data_;

      size_t index(size_t row, size_t col) const;
  };

  Matrix concatRows(const kondrat::Vector< Matrix > & matrices);
  Matrix concatCols(const kondrat::Vector< Matrix > & matrices);
  Matrix concatMainDiag(const kondrat::Vector< Matrix > & matrices);
  Matrix concatSideDiag(const kondrat::Vector< Matrix > & matrices);
  Matrix operator*(ll number, const Matrix & matrix);
  std::ostream & operator<<(std::ostream & out, const Matrix & matrix);
}

#endif

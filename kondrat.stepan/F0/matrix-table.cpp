#include "matrix-table.hpp"
#include <fstream>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector/top-it-vector.hpp>

namespace
{
  template< class T >
  void sortVector(kondrat::Vector< T > & values)
  {
    for (size_t i = 0; i < values.getSize(); ++i)
    {
      for (size_t j = i + 1; j < values.getSize(); ++j)
      {
        if (values[j] < values[i])
        {
          const T tmp = values[i];
          values[i] = values[j];
          values[j] = tmp;
        }
      }
    }
  }

  size_t readIndex(std::istream & in)
  {
    long long value = 0;
    if (!(in >> value) || value < 0)
    {
      throw std::logic_error("invalid index");
    }
    return static_cast< size_t >(value);
  }

  size_t readMatrixCount(std::istream & in)
  {
    const size_t count = readIndex(in);
    if (count < 2)
    {
      throw std::logic_error("invalid matrix count");
    }
    return count;
  }

  template< class Storage >
  kondrat::Vector< kondrat::Matrix > readMatrices(
    std::istream & in, const Storage & storage, size_t count)
  {
    kondrat::Vector< kondrat::Matrix > matrices;
    for (size_t i = 0; i < count; ++i)
    {
      std::string name;
      if (!(in >> name))
      {
        throw std::logic_error("invalid command");
      }
      matrices.pushBack(storage.at(name));
    }
    return matrices;
  }
}

void kondrat::MatrixTable::create(std::istream & in, std::ostream &, std::string matrixName)
{
  const size_t rows = readIndex(in);
  const size_t cols = readIndex(in);
  if (rows == 0 || cols == 0 || hasExtraArgs(in) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, Matrix(rows, cols));
}

void kondrat::MatrixTable::drop(std::istream & in, std::ostream &, std::string matrixName)
{
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  if (matrices_.erase(matrixName) == 0)
  {
    throw std::logic_error("invalid command");
  }
}

void kondrat::MatrixTable::show(std::istream & in, std::ostream & out, std::string matrixName)
{
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  const Matrix & matrix = matrices_.at(matrixName);
  out << matrixName << ":\n" << matrix << '\n';
}

void kondrat::MatrixTable::list(std::istream & in, std::ostream & out, std::string)
{
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  if (matrices_.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  kondrat::Vector< std::string > names;
  for (Storage::ConstIterator it = matrices_.cbegin(); it != matrices_.cend(); ++it)
  {
    names.pushBack(it->first);
  }
  sortVector(names);

  for (size_t i = 0; i < names.getSize(); ++i)
  {
    const Matrix & matrix = matrices_.at(names[i]);
    out << names[i] << ' ' << matrix.rows() << 'x' << matrix.cols() << '\n';
  }
}

void kondrat::MatrixTable::rename(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string newName;
  if (!(in >> newName) || hasExtraArgs(in) || matrices_.contains(newName))
  {
    throw std::logic_error("invalid command");
  }

  Storage copy(matrices_);
  const Matrix matrix = copy.at(matrixName);
  copy.erase(matrixName);
  copy.insert(newName, matrix);
  matrices_.swap(copy);
}

void kondrat::MatrixTable::set(std::istream & in, std::ostream &, std::string matrixName)
{
  const size_t row = readIndex(in);
  const size_t col = readIndex(in);
  ll value = 0;
  if (!(in >> value) || hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  Matrix matrix = matrices_.at(matrixName);
  matrix.at(row, col) = value;
  matrices_.insert(matrixName, matrix);
}

void kondrat::MatrixTable::addMany(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }

  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  Matrix result = matrices[0];
  for (size_t i = 1; i < matrices.getSize(); ++i)
  {
    result = result + matrices[i];
  }
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::subMany(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }

  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  Matrix result = matrices[0];
  for (size_t i = 1; i < matrices.getSize(); ++i)
  {
    result = result - matrices[i];
  }
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::mulMany(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }

  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  Matrix result = matrices[0];
  for (size_t i = 1; i < matrices.getSize(); ++i)
  {
    result = result * matrices[i];
  }
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::mulNumber(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string sourceName;
  ll number = 0;
  if (!(in >> sourceName >> number) || hasExtraArgs(in) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, matrices_.at(sourceName) * number);
}

void kondrat::MatrixTable::pow(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string sourceName;
  if (!(in >> sourceName) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t degree = readIndex(in);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, matrices_.at(sourceName).getPower(degree));
}

void kondrat::MatrixTable::insertRow(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string targetName;
  if (!(in >> targetName) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t targetRow = readIndex(in);
  std::string sourceName;
  if (!(in >> sourceName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t sourceRow = readIndex(in);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  const Matrix result = matrices_.at(targetName).getInsertedRow(targetRow, matrices_.at(sourceName), sourceRow);
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::insertCol(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string targetName;
  if (!(in >> targetName) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t targetCol = readIndex(in);
  std::string sourceName;
  if (!(in >> sourceName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t sourceCol = readIndex(in);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  const Matrix result = matrices_.at(targetName).getInsertedCol(targetCol, matrices_.at(sourceName), sourceCol);
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::insert(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string targetName;
  std::string sourceName;
  if (!(in >> targetName >> sourceName) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t startRow = readIndex(in);
  const size_t startCol = readIndex(in);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  const Matrix result = matrices_.at(targetName).getInserted(matrices_.at(sourceName), startRow, startCol);
  matrices_.insert(matrixName, result);
}

void kondrat::MatrixTable::concatRow(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }
  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, concatRows(matrices));
}

void kondrat::MatrixTable::concatCol(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }
  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, concatCols(matrices));
}

void kondrat::MatrixTable::concatMainDiag(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }
  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, kondrat::concatMainDiag(matrices));
}

void kondrat::MatrixTable::concatSideDiag(std::istream & in, std::ostream &, std::string matrixName)
{
  if (matrices_.contains(matrixName))
  {
    throw std::logic_error("name is occupied");
  }
  const size_t count = readMatrixCount(in);
  const kondrat::Vector< Matrix > matrices = readMatrices(in, matrices_, count);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, kondrat::concatSideDiag(matrices));
}

void kondrat::MatrixTable::minor(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string sourceName;
  if (!(in >> sourceName) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }
  const size_t row = readIndex(in);
  const size_t col = readIndex(in);
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  matrices_.insert(matrixName, matrices_.at(sourceName).getMinor(row, col));
}

void kondrat::MatrixTable::det(std::istream & in, std::ostream & out, std::string matrixName)
{
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  out << matrices_.at(matrixName).getDeterminant() << '\n';
}

void kondrat::MatrixTable::rank(std::istream & in, std::ostream & out, std::string matrixName)
{
  if (hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }
  out << matrices_.at(matrixName).getRank() << '\n';
}

void kondrat::MatrixTable::compare(std::istream & in, std::ostream & out, std::string matrixName)
{
  std::string secondName;
  if (!(in >> secondName) || hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  if (matrices_.at(matrixName) == matrices_.at(secondName))
  {
    out << "<EQUAL>\n";
  }
  else
  {
    out << "<NOT EQUAL>\n";
  }
}

void kondrat::MatrixTable::saveMatrix(std::istream & in, std::ostream & out, std::string matrixName)
{
  std::string fileName;
  if (!(in >> fileName) || hasExtraArgs(in))
  {
    throw std::logic_error("invalid command");
  }

  const Matrix & matrix = matrices_.at(matrixName);
  std::ofstream file(fileName);
  if (!file)
  {
    throw std::runtime_error("cannot open file");
  }

  file << matrix.rows() << ' ' << matrix.cols() << '\n';
  file << matrix << '\n';
  if (!file)
  {
    throw std::runtime_error("cannot write file");
  }
  out << "<OK>\n";
}

void kondrat::MatrixTable::loadMatrix(std::istream & in, std::ostream &, std::string matrixName)
{
  std::string fileName;
  if (!(in >> fileName) || hasExtraArgs(in) || matrices_.contains(matrixName))
  {
    throw std::logic_error("invalid command");
  }

  std::ifstream file(fileName);
  if (!file)
  {
    throw std::runtime_error("cannot open file");
  }

  const size_t rows = readIndex(file);
  const size_t cols = readIndex(file);
  if (rows == 0 || cols == 0)
  {
    throw std::logic_error("invalid matrix file");
  }

  Matrix matrix(rows, cols);
  for (Matrix::Iterator it = matrix.begin(); it != matrix.end(); ++it)
  {
    if (!(file >> *it))
    {
      throw std::logic_error("invalid matrix file");
    }
  }
  std::string extra;
  if (file >> extra)
  {
    throw std::logic_error("invalid matrix file");
  }
  matrices_.insert(matrixName, matrix);
}

bool kondrat::MatrixTable::hasExtraArgs(std::istream & in) const
{
  while (in)
  {
    const int next = in.peek();
    if (next == '\n' || next == std::char_traits< char >::eof())
    {
      return false;
    }

    const int symbol = in.get();
    if (symbol != ' ' && symbol != '\t' && symbol != '\r')
    {
      return true;
    }
  }
  return false;
}

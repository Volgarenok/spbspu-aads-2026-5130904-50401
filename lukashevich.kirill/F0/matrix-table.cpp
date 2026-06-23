#include "matrix-table.hpp"

#include <ostream>
#include <stdexcept>

lukashevich::MatrixTable::MatrixTable():
  matrices_(),
  names_()
{}

bool lukashevich::MatrixTable::contains(const std::string & name) const
{
  return matrices_.contains(name);
}

void lukashevich::MatrixTable::add(const std::string & name, const Matrix & matrix)
{
  if (contains(name)) {
    throw std::invalid_argument("matrix exists");
  }

  matrices_.insert(name, matrix);

  try {
    names_.pushBack(name);
  } catch (...) {
    matrices_.erase(name);
    throw;
  }
}

void lukashevich::MatrixTable::remove(const std::string & name)
{
  if (!contains(name)) {
    throw std::invalid_argument("matrix not found");
  }

  eraseName(name);
  matrices_.erase(name);
}

lukashevich::Matrix & lukashevich::MatrixTable::get(const std::string & name)
{
  return matrices_.at(name);
}

const lukashevich::Matrix & lukashevich::MatrixTable::get(const std::string & name) const
{
  return matrices_.at(name);
}

void lukashevich::MatrixTable::printList(std::ostream & out) const
{
  out << "Stored: ";
  if (names_.isEmpty()) {
    out << "(empty)\n";
    return;
  }

  for (size_t i = 0; i < names_.getSize(); ++i) {
    const std::string & name = names_[i];
    const Matrix & matrix = matrices_.at(name);

    if (i != 0) {
      out << ", ";
    }

    out << name << " [" << matrix.getRows() << "x" << matrix.getCols() << "]";
  }

  out << '\n';
}

void lukashevich::MatrixTable::printOne(std::ostream & out,
    const std::string & name) const
{
  printMatrix(out, name, get(name));
}

void lukashevich::MatrixTable::eraseName(const std::string & name)
{
  for (size_t i = 0; i < names_.getSize(); ++i) {
    if (names_[i] == name) {
      names_.erase(i);
      return;
    }
  }

  throw std::invalid_argument("matrix not found");
}

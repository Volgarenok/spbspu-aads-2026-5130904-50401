#ifndef MATRIX_TABLE_HPP
#define MATRIX_TABLE_HPP

#include <iosfwd>
#include <string>

#include "vector/vector.hpp"
#include "hash-func.hpp"
#include "double-hash-table.hpp"
#include "matrix.hpp"

namespace lukashevich {
  class MatrixTable
  {
  public:
    MatrixTable();

    bool contains(const std::string & name) const;
    void add(const std::string & name, const Matrix & matrix);
    void remove(const std::string & name);

    Matrix & get(const std::string & name);
    const Matrix & get(const std::string & name) const;

    void printList(std::ostream & out) const;
    void printOne(std::ostream & out, const std::string & name) const;

  private:
    void eraseName(const std::string & name);

    HashTable< std::string, Matrix, Hash1, Hash2 > matrices_;
    Vector< std::string > names_;
  };
}

#endif

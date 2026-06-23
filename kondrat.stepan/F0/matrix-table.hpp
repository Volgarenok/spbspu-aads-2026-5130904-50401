#ifndef MATRIX_TABLE_HPP
#define MATRIX_TABLE_HPP

#include <iosfwd>
#include <string>
#include "cuckoo-hash-table.hpp"
#include "hash-func.hpp"
#include "matrix.hpp"

namespace kondrat
{
  class MatrixTable
  {
    public:
      void create(std::istream & in, std::ostream & out, std::string matrixName);
      void drop(std::istream & in, std::ostream & out, std::string matrixName);
      void show(std::istream & in, std::ostream & out, std::string matrixName);
      void list(std::istream & in, std::ostream & out, std::string matrixName);
      void rename(std::istream & in, std::ostream & out, std::string matrixName);
      void set(std::istream & in, std::ostream & out, std::string matrixName);

      void addMany(std::istream & in, std::ostream & out, std::string matrixName);
      void subMany(std::istream & in, std::ostream & out, std::string matrixName);
      void mulMany(std::istream & in, std::ostream & out, std::string matrixName);
      void mulNumber(std::istream & in, std::ostream & out, std::string matrixName);
      void pow(std::istream & in, std::ostream & out, std::string matrixName);

      void insertRow(std::istream & in, std::ostream & out, std::string matrixName);
      void insertCol(std::istream & in, std::ostream & out, std::string matrixName);
      void insert(std::istream & in, std::ostream & out, std::string matrixName);

      void concatRow(std::istream & in, std::ostream & out, std::string matrixName);
      void concatCol(std::istream & in, std::ostream & out, std::string matrixName);
      void concatMainDiag(std::istream & in, std::ostream & out, std::string matrixName);
      void concatSideDiag(std::istream & in, std::ostream & out, std::string matrixName);

      void minor(std::istream & in, std::ostream & out, std::string matrixName);
      void det(std::istream & in, std::ostream & out, std::string matrixName);
      void rank(std::istream & in, std::ostream & out, std::string matrixName);
      void compare(std::istream & in, std::ostream & out, std::string matrixName);

      void saveMatrix(std::istream & in, std::ostream & out, std::string matrixName);
      void loadMatrix(std::istream & in, std::ostream & out, std::string matrixName);

    private:
      using Storage = CuckooHashTable< std::string, Matrix, Hash1, Hash2 >;

      Storage matrices_;

      bool hasExtraArgs(std::istream & in) const;
  };
}

#endif

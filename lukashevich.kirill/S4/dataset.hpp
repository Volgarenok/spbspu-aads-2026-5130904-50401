#ifndef DATASET_HPP
#define DATASET_HPP

#include "compare.hpp"
#include "tree.hpp"

#include <functional>
#include <iosfwd>
#include <string>

namespace lukashevich
{
  using DataSet = BSTree< int, std::string, Comparator< int > >;
  using DataSetTable = BSTree< std::string, DataSet, Comparator< std::string > >;

  class DatasetTable
  {
  public:
    DatasetTable() = default;

    void set(const std::string & name, const DataSet & dataSet);
    const DataSet & at(const std::string & name) const;

    void print(const std::string & name, std::ostream & output) const;
    void complement(const std::string & newName,
        const std::string & lhsName, const std::string & rhsName);
    void intersect(const std::string & newName,
        const std::string & lhsName, const std::string & rhsName);
    void unite(const std::string & newName,
        const std::string & lhsName, const std::string & rhsName);

  private:
    DataSetTable dataSets_;
  };
}

#endif

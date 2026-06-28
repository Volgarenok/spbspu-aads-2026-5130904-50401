#ifndef DATASET_TABLE_HPP
#define DATASET_TABLE_HPP

#include "../common/my_tree/my_compare.hpp"
#include "../common/my_tree/my_tree.hpp"

#include <iostream>
#include <string>

#include "../common/my_vector/top-it-vector.hpp"
namespace zubarev
{
  class DatasetTable
  {

  private:
    using bst_datasets =
        BSTree< std::string, BSTree< size_t, std::string, Comparator< size_t > >, Comparator< std::string > >;
    bst_datasets datasetsTable;
    static bool compare(const std::pair< std::string, Vector< size_t > >& a,
                        const std::pair< std::string, Vector< size_t > >& b)
    {
      return a.first < b.first;
    }

  public:
    void print(const std::string&, std::ostream&) const;
    void complement(const std::string&, const std::string&, const std::string&);
    void intersect(const std::string&, const std::string&, const std::string&);
    void union_fun(const std::string&, const std::string&, const std::string&);
    void insertDataset(const std::string& name, const BSTree< size_t, std::string, Comparator< size_t > >& dataset);
  };

}

#endif

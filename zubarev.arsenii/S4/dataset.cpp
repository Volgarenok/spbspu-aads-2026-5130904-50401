#include "dataset.hpp"
namespace zubarev
{
  void DatasetTable::insertDataset(const std::string& name,
                                   const BSTree< size_t, std::string, Comparator< size_t > >& dataset)
  {
    datasetsTable.push(name, dataset);
  }
  void DatasetTable::print(const std::string& name, std::ostream& out) const
  {
    const BSTree< size_t, std::string, Comparator< size_t > >& dataset = datasetsTable.at(name);
    if (dataset.empty()) {
      out << "<EMPTY>\n";
      return;
    }
    out << name;
    for (auto it = dataset.begin(); it != dataset.end(); ++it) {
      out << " " << it->first << " " << it->second;
    }
    out << '\n';
  }
  void DatasetTable::complement(const std::string& new_name, const std::string& name1, const std::string& name2)
  {
    BSTree< size_t, std::string, Comparator< size_t > > new_tree;
    const BSTree< size_t, std::string, Comparator< size_t > >& data1 = datasetsTable.at(name1);
    const BSTree< size_t, std::string, Comparator< size_t > >& data2 = datasetsTable.at(name2);
    for (auto it1 = data1.begin(); it1 != data1.end(); ++it1) {
      if (!data2.contains(it1->first)) {
        new_tree.push(it1->first, it1->second);
      }
    }
    datasetsTable[new_name] = new_tree;
  }
  void DatasetTable::intersect(const std::string& new_name, const std::string& name1, const std::string& name2)
  {
    BSTree< size_t, std::string, Comparator< size_t > > new_tree;
    const BSTree< size_t, std::string, Comparator< size_t > >& data1 = datasetsTable.at(name1);
    const BSTree< size_t, std::string, Comparator< size_t > >& data2 = datasetsTable.at(name2);
    for (auto it1 = data1.begin(); it1 != data1.end(); ++it1) {
      if (data2.contains(it1->first)) {
        new_tree.push(it1->first, it1->second);
      }
    }
    datasetsTable[new_name] = new_tree;
  }
  void DatasetTable::union_fun(const std::string& new_name, const std::string& name1, const std::string& name2)
  {
    BSTree< size_t, std::string, Comparator< size_t > > new_tree;
    const BSTree< size_t, std::string, Comparator< size_t > >& data1 = datasetsTable.at(name1);
    const BSTree< size_t, std::string, Comparator< size_t > >& data2 = datasetsTable.at(name2);
    for (auto it1 = data1.begin(); it1 != data1.end(); ++it1) {
      new_tree.push(it1->first, it1->second);
    }
    for (auto it2 = data2.begin(); it2 != data2.end(); ++it2) {
      if (!new_tree.contains(it2->first)) {
        new_tree.push(it2->first, it2->second);
      }
    }
    datasetsTable[new_name] = new_tree;
  }
}

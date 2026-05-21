#include "commands.hpp"
#include "dataset.hpp"
#include <iostream>

namespace zubarev
{

  void cmd_print(std::istream& in, std::ostream& out, DatasetTable& dataset)
  {
    std::string name;
    in >> name;
    dataset.print(name, out);
  }
  void cmd_complement(std::istream& in, std::ostream&, DatasetTable& dataset)
  {
    std::string new_name, name1, name2;
    in >> new_name >> name1 >> name2;
    dataset.complement(new_name, name1, name2);
  }
  void cmd_intersect(std::istream& in, std::ostream&, DatasetTable& dataset)
  {
    std::string new_name, name1, name2;
    in >> new_name >> name1 >> name2;
    dataset.intersect(new_name, name1, name2);
  }
  void cmd_union(std::istream& in, std::ostream&, DatasetTable& dataset)
  {
    std::string new_name, name1, name2;
    in >> new_name >> name1 >> name2;
    dataset.union_fun(new_name, name1, name2);
  }
}

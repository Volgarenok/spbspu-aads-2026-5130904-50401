#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "cuckoo-hash-table.hpp"
#include "hash-func.hpp"
#include "matrix-table.hpp"

int main()
{
  using Command = void(kondrat::MatrixTable::*)(std::istream &, std::ostream &, std::string);
  using CommandTable = kondrat::CuckooHashTable< std::string, Command, kondrat::Hash1, kondrat::Hash2 >;

  kondrat::MatrixTable table;
  CommandTable commands;

  commands.insert("create", &kondrat::MatrixTable::create);
  commands.insert("drop", &kondrat::MatrixTable::drop);
  commands.insert("show", &kondrat::MatrixTable::show);
  commands.insert("list", &kondrat::MatrixTable::list);
  commands.insert("rename", &kondrat::MatrixTable::rename);
  commands.insert("set", &kondrat::MatrixTable::set);
  commands.insert("add-many", &kondrat::MatrixTable::addMany);
  commands.insert("sub-many", &kondrat::MatrixTable::subMany);
  commands.insert("mul-many", &kondrat::MatrixTable::mulMany);
  commands.insert("mul-number", &kondrat::MatrixTable::mulNumber);
  commands.insert("pow", &kondrat::MatrixTable::pow);
  commands.insert("insert-row", &kondrat::MatrixTable::insertRow);
  commands.insert("insert-col", &kondrat::MatrixTable::insertCol);
  commands.insert("insert", &kondrat::MatrixTable::insert);
  commands.insert("concat-row", &kondrat::MatrixTable::concatRow);
  commands.insert("concat-col", &kondrat::MatrixTable::concatCol);
  commands.insert("concat-main-diag", &kondrat::MatrixTable::concatMainDiag);
  commands.insert("concat-side-diag", &kondrat::MatrixTable::concatSideDiag);
  commands.insert("minor", &kondrat::MatrixTable::minor);
  commands.insert("det", &kondrat::MatrixTable::det);
  commands.insert("rank", &kondrat::MatrixTable::rank);
  commands.insert("compare", &kondrat::MatrixTable::compare);
  commands.insert("save-matrix", &kondrat::MatrixTable::saveMatrix);
  commands.insert("load-matrix", &kondrat::MatrixTable::loadMatrix);

  std::string command;

  while (std::cin >> command)
  {
    if (command == "exit")
    {
      break;
    }

    try
    {
      const Command method = commands.at(command);
      std::string matrixName;
      if (command != "list" && !(std::cin >> matrixName))
      {
        throw std::logic_error("invalid command");
      }
      (table.*method)(std::cin, std::cout, matrixName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
    }

    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

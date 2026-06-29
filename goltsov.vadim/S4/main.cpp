#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "mybst.hpp"
#include "sets_functions.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    return 1;
  }

  std::fstream file_in (argv[1]);

  goltsov::bst_s_bst_is all_sets;

  goltsov::readSets(file_in, all_sets);

  goltsov::bst_func functions;
  functions.insert(std::pair< std::string, void (*)(std::ostream&, std::istream&, goltsov::bst_s_bst_is&) >{"print",
    goltsov::printParsing});
  functions.insert(std::pair< std::string, void (*)(std::ostream&, std::istream&, goltsov::bst_s_bst_is&) >{"complement",
    goltsov::complementParsing});
  functions.insert(std::pair< std::string, void (*)(std::ostream&, std::istream&, goltsov::bst_s_bst_is&) >{"intersect",
    goltsov::intersectParsing});
  functions.insert(std::pair< std::string, void (*)(std::ostream&, std::istream&, goltsov::bst_s_bst_is&) >{"union",
    goltsov::unionParsing});
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      functions.at(command)(std::cout, std::cin, all_sets);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail() && !std::cin.eof())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

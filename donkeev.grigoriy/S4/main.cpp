#include <iostream>
#include <fstream>
#include <limits>

#include "BSTree.hpp"
#include "trees-commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "There no filename\n";
    return 1;
  }
  std::string filename(argv[1]);
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Сannot open file\n";
    return 1;
  }

  donkeev::Datasets allDicts;

  std::string dictName;
  while (file >> dictName)
  {
    donkeev::Dataset dict;

    size_t key;
    std::string value;
    while (file >> key >> value)
    {
      dict.insert(key, value);
    }

    file.clear();

    allDicts.insert(dictName, std::move(dict));
  }

  file.close();

  using cmd_t = void(*)(std::istream&, std::ostream&, donkeev::Datasets&);
  donkeev::BSTree< std::string, cmd_t, donkeev::Comp< std::string > > commands;

  commands.insert("print", donkeev::printDicts);
  commands.insert("complement", donkeev::complementDicts);
  commands.insert("intersect", donkeev::intersectDicts);
  commands.insert("union", donkeev::uniteDicts);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands[command](std::cin, std::cout, allDicts);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

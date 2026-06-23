#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "commands.hpp"
#include "bstree.hpp"

namespace
{
  bool tryParseInt(const std::string& token, int& result)
  {
    if (token.empty())
    {
      return false;
    }

    size_t pos = 0;
    try
    {
      result = std::stoi(token, std::addressof(pos));
      return pos == token.size();
    }
    catch (...)
    {
      return false;
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Too much" << '\n';
    return 1;
  }

  vasyakin::Datasets datasets;

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  std::string token;
  std::string currDataset;

  while (file >> token)
  {
    int key = 0;
    if (tryParseInt(token, key))
    {
      if (currDataset.empty())
      {
        continue;
      }

      std::string value;
      if (file >> value)
      {
        datasets.at(currDataset).insert(key, value);
      }
    }
    else
    {
      currDataset = token;

      try
      {
        datasets.at(currDataset);
      }
      catch (const std::out_of_range&)
      {
        datasets.insert(currDataset, vasyakin::Dataset{});
      }
    }
  }
  file.close();

  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::Datasets&);
  vasyakin::BSTree< std::string, cmd_t > cmds;

  cmds.insert("print", vasyakin::cmdPrint);
  cmds.insert("complement", vasyakin::cmdComplement);
  cmds.insert("intersect", vasyakin::cmdIntersect);
  cmds.insert("union", vasyakin::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, datasets);
      std::cout << '\n';
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}

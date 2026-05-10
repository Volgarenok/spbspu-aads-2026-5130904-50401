#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "commands.hpp"
#include "bstree.hpp"

namespace vasyakin
{
  bool isInteger(const std::string& token)
  {
    if (token.empty())
    {
      return false;
    }

    size_t pos = 0;
    try
    {
      std::stoi(token, &pos);
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
    if (vasyakin::isInteger(token))
    {
      if (currDataset.empty())
      {
        continue;
      }

      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        datasets.get(currDataset).push(key, value);
      }
    }
    else
    {
      currDataset = token;

      try
      {
        datasets.get(currDataset);
      }
      catch (const std::out_of_range&)
      {
        datasets.push(currDataset, vasyakin::Dataset{});
      }
    }
  }
  file.close();

  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::Datasets&);
  vasyakin::BSTree< std::string, cmd_t > cmds;

  cmds.push("print", vasyakin::cmdPrint);
  cmds.push("complement", vasyakin::cmdComplement);
  cmds.push("intersect", vasyakin::cmdIntersect);
  cmds.push("union", vasyakin::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cin, std::cout, datasets);
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

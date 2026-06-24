#include "commands.hpp"

#include <iostream>
#include <limits>
#include <string>

#include "AVLTree.hpp"


int main()
{
  using CommandFunc = void (*)(std::istream&, std::ostream&, burukov::LibraryManager&);
  burukov::AVLTree< std::string, CommandFunc > commands;
  commands.push("add-title", burukov::parsingAddTitle);
  commands.push("add-copy", burukov::parsingAddCopy);
  commands.push("lend", burukov::parsingLend);
  commands.push("return", burukov::parsingReturn);
  commands.push("demand-model-title", burukov::parsingDemandTitle);
  commands.push("demand-model-genre", burukov::parsingDemandGenre);
  commands.push("slice-title", burukov::parsingSliceTitle);
  commands.push("slice-genre", burukov::parsingSliceGenre);
  commands.push("recommend", burukov::parsingRecommend);
  commands.push("dead-stock", burukov::parsingDeadStock);
  commands.push("demand-balance", burukov::parsingDemandBalance);
  burukov::LibraryManager lib;
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (commands.hasKey(cmd))
      {
        commands.at(cmd)(std::cin, std::cout, lib);
      }
      else
      {
        throw std::runtime_error("unknown command");
      }
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}

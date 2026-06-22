#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <bstree.hpp>
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Not enough args\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input.is_open())
  {
    std::cerr << "Can not open file\n";
    return 1;
  }

  zharov::BSTree< std::string, zharov::cmd, std::less< std::string > > cmds;
  zharov::BSTree< std::string, zharov::constCmd, std::less< std::string > > constCmds;
  constCmds.push("print", zharov::print);
  cmds.push("complement", zharov::complement);
  cmds.push("intersect", zharov::intersect);
  cmds.push("union", zharov::makeUnion);

  zharov::dictionaries_t dicts;
  std::string dict_str;
  while (getline(input, dict_str))
  {
    zharov::dictionary_t dict;
    std::string dict_name;
    size_t i = 0;
    for (; i < dict_str.size() && dict_str[i] != ' '; ++i)
    {
      dict_name += dict_str[i];
    }
    ++i;
    for (; i < dict_str.size(); ++i)
    {
      std::string key;
      for (; i < dict_str.size() && dict_str[i] != ' '; ++i)
      {
        key += dict_str[i];
      }
      ++i;
      std::string value;
      for (; i < dict_str.size() && dict_str[i] != ' '; ++i)
      {
        value += dict_str[i];
      }
      dict.push(std::stoi(key), value);
    }
    dicts.push(dict_name, dict);
  }

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (cmds.has(command))
      {
        cmds.at(command)(std::cout, std::cin, dicts);
      }
      else
      {
        constCmds.at(command)(std::cout, std::cin, dicts);
        std::cout << '\n';
      }
    }
    catch (...)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
}

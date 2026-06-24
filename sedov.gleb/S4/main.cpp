#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"

namespace
{
  bool isInteger(const std::string & str)
  {
    if (str.empty())
    {
      return false;
    }
    try
    {
      size_t pos = 0;
      std::stoi(str, std::addressof(pos));
      return pos == str.size();
    }
    catch (const std::exception &)
    {
      return false;
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "bad num of args" << '\n';
    return 1;
  }
  sedov::trees trees;
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }
  std::string token;
  std::string curp;
  while (file >> token)
  {
    if (isInteger(token))
    {
      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        try
        {
          trees.at(curp);
        }
        catch (const std::out_of_range &)
        {
          trees.insert(curp, sedov::pairs{});
        }
        trees.at(curp).insert(key, value);
      }
    }
    else
    {
      curp = token;
      try
      {
        trees.at(curp);
      }
      catch (const std::out_of_range &)
      {
        trees.insert(curp, sedov::pairs{});
      }
    }
  }
  file.close();
  using cmdT = void (*)(std::istream &, std::ostream &, sedov::trees &);
  sedov::BSTree< std::string, cmdT > mutableCmds;
  mutableCmds.insert("complement", sedov::complement);
  mutableCmds.insert("intersect", sedov::intersect);
  mutableCmds.insert("union", sedov::cUnion);

  sedov::BSTree< std::string, cmdT > constCmds;
  constCmds.insert("print", sedov::print);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (mutableCmds.count(cmd) > 0)
      {
        mutableCmds.at(cmd)(std::cin, std::cout, trees);
      }
      else if (constCmds.count(cmd) > 0)
      {
        constCmds.at(cmd)(std::cin, std::cout, trees);
        std::cout << '\n';
      }
      else
      {
        throw std::runtime_error("Unknown command");
      }
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}

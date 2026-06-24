#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"

namespace burukov
{
  bool isInteger(const std::string& s)
  {
    if (s.empty())
    {
      return false;
    }

    size_t i = 0;
    if (s[0] == '-' || s[0] == '+')
    {
      i = 1;
    }
    if (i == s.size())
    {
      return false;
    }

    for (size_t j = i; j < s.size(); ++j)
    {
      if (s[j] < '0' || s[j] > '9')
      {
        return false;
      }
    }
    return true;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "usage: ./lab <file>" << '\n';
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "cannot open file: " << argv[1] << '\n';
    return 1;
  }

  burukov::Dicts dicts;
  std::string token;
  std::string current;

  while (file >> token)
  {
    if (burukov::isInteger(token))
    {
      if (current.empty())
      {
        continue;
      }

      int key = std::stoi(token);
      std::string val;
      if (file >> val)
      {
        try
        {
          dicts.at(current).push(key, val);
        }
        catch (const std::out_of_range&)
        {
          if (!dicts.hasKey(current))
          {
            dicts.push(current, burukov::Dict{});
          }
          dicts.at(current).push(key, val);
        }
      }
    }
    else
    {
      current = token;
      if (!dicts.hasKey(current))
      {
        dicts.push(current, burukov::Dict{});
      }
    }
  }
  file.close();

  using Cmd = void (*)(std::istream&, std::ostream&, burukov::Dicts&);
  burukov::BSTree< std::string, Cmd > commands;

  commands.push("print", burukov::cmdPrint);
  commands.push("complement", burukov::cmdComplement);
  commands.push("intersect", burukov::cmdIntersect);
  commands.push("union", burukov::cmdUnion);

  std::string line;
  while (std::cin >> line)
  {
    try
    {
      commands.at(line)(std::cin, std::cout, dicts);
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

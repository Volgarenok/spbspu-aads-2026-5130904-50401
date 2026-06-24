#include "bstree.hpp"
#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace
{
  bool isInteger(const std::string& str)
  {
    if (str.empty())
    {
      return false;
    }

    size_t pos = 0;
    if (str[pos] == '-' || str[pos] == '+')
    {
      ++pos;
    }

    if (pos == str.size())
    {
      return false;
    }

    while (pos < str.size())
    {
      if (str[pos] < '0' || str[pos] > '9')
      {
        return false;
      }
      ++pos;
    }
    return true;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "invalid argument";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "cannot open file\n";
    return 1;
  }

  studilova::Datasets datasets;

  std::string token;
  std::string current_name;

  while (file >> token)
  {
    if (!isInteger(token))
    {
      current_name = token;
      try
      {
        datasets.get(current_name);
      }
      catch(const std::out_of_range&)
      {
        datasets.push(current_name, studilova::Dataset());
      }
    } else {
      if (current_name.empty())
      {
        continue;
      }

      int key = std::stoi(token);

      std::string value;
      if (!(file >> value))
      {
        break;
      }
      datasets.get(current_name).push(key, value);
    }
  }

  using Command = void(*)(std::istream&, std::ostream&, studilova::Datasets&);
  studilova::BSTree< std::string, Command > commands;

  commands.push("print", studilova::Print);
  commands.push("complement", studilova::Complement);
  commands.push("intersect", studilova::Intersect);
  commands.push("union", studilova::cmdUnion);

  std::string command_name;

  while (std::cin >> command_name)
  {
    try
    {
      commands.get(command_name)(std::cin, std::cout, datasets);
    }
    catch(const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}

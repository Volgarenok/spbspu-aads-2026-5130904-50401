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
  return 0;
}

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

int main()
{
  return 0;
}

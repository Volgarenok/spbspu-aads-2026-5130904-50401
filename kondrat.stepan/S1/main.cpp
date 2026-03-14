#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"

namespace kondrat
{
  using pairN = std::pair< std::string, List< size_t > >;

  void getData(std::istream & in, List< pairN > & res)
  {
    std::string name;

    while (in >> name)
    {
      List< size_t > nums;
      size_t num = 0;

      while (in >> num)
      {
        nums.pushBack(num);
      }

      pairN p(name, nums);
      res.pushBack(p);

      if (in.fail() && !in.eof())
      {
        in.clear();
      }
    }
  }

  void printNames(const List< pairN > & data)
  {
    bool first = true;

    LCIter< pairN > it = data.begin();
    LCIter< pairN > end = data.end();

    while (it != end)
    {
      if (!first)
      {
        std::cout << ' ';
      }

      std::cout << (*it).first;

      first = false;
      ++it;
    }

    std::cout << '\n';
  }

  bool hasNumbers(List< pairN > & data)
  {
    LIter< pairN > it = data.begin();
    LIter< pairN > end = data.end();

    while (it != end)
    {
      if (!((*it).second.empty()))
      {
        return true;
      }
      ++it;
    }

    return false;
  }

  size_t printOneRow(List< pairN > & data)
  {
    LIter< pairN > it = data.begin();
    LIter< pairN > end = data.end();

    bool first = true;
    size_t sum = 0;

    while (it != end)
    {
      if (!((*it).second.empty()))
      {
        size_t value = (*it).second.front();

        if (!first)
        {
          std::cout << ' ';
        }

        std::cout << value;
        sum += value;
        (*it).second.popFront();

        first = false;
      }
      ++it;
    }

    std::cout << '\n';
    return sum;
  }
}
#include <iostream>
#include <string>
#include <limits>
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

  bool hasNumbers(const List< pairN > & data)
  {
    LCIter< pairN > it = data.begin();
    LCIter< pairN > end = data.end();

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

  bool printOneRow(List< pairN > & data, size_t & sum)
  {
    LIter< pairN > it = data.begin();
    LIter< pairN > end = data.end();

    bool first = true;
    sum = 0;

    while (it != end)
    {
      if (!((*it).second.empty()))
      {
        size_t value = (*it).second.front();

        if (sum > std::numeric_limits< size_t >::max() - value)
        {
          return false;
        }

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
    return true;
  }

  bool printRowsAndSums(List< pairN > & data, List< size_t > & sums)
  {
    while (hasNumbers(data))
    {
      size_t sum = 0;

      if (!printOneRow(data, sum))
      {
        return false;
      }

      sums.pushBack(sum);
    }

    return true;
  }

  void printSums(const List< size_t > & sums)
  {
    if (sums.empty())
    {
      std::cout << 0 << '\n';
      return;
    }

    bool first = true;

    LCIter< size_t > it = sums.begin();
    LCIter< size_t > end = sums.end();

    while (it != end)
    {
      if (!first)
      {
        std::cout << ' ';
      }

      std::cout << *it;

      first = false;
      ++it;
    }

    std::cout << '\n';
  }
}

int main()
{
  kondrat::List< kondrat::pairN > data;

  kondrat::getData(std::cin, data);

  if (data.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  kondrat::printNames(data);

  kondrat::List< kondrat::pairN > copy(data);
  kondrat::List< size_t > sums;

  if (!kondrat::printRowsAndSums(copy, sums))
  {
    std::cerr << "Error: sum overflow\n";
    return 1;
  }

  kondrat::printSums(sums);

  return 0;
}

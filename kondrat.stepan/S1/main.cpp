#include <iostream>
#include <string>
#include <stdexcept>
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

  size_t printOneRow(List< pairN > & data)
  {
    LIter< pairN > it = data.begin();
    LIter< pairN > end = data.end();

    bool first = true;
    bool overflow = false;

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

        if (!overflow)
        {
          if (sum > std::numeric_limits<size_t>::max() - value)
          {
            overflow = true;
          }
          else
          {
            sum += value;
          }
        }

        (*it).second.popFront();
        first = false;
      }

      ++it;
    }

    std::cout << '\n';

    if (overflow)
    {
      throw std::overflow_error("overflow");
    }

    return sum;
  }

  void printRowsAndSums(List< pairN > & data, List< size_t > & sums)
  {
    while (hasNumbers(data))
    {
      size_t sum = printOneRow(data);
      sums.pushBack(sum);
    }
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

  try
  {
    kondrat::printRowsAndSums(copy, sums);
  }
  catch (const std::overflow_error &)
  {
    std::cerr << "ERROR\n";
    return 1;
  }

  kondrat::printSums(sums);

  return 0;
}

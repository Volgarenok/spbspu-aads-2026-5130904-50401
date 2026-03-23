#include "functions.hpp"

namespace lukashevich
{
  void getData(std::istream& in, List< pair_t >& res)
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

      pair_t p(name, nums);
      res.pushBack(p);

      if (in.fail() && !in.eof())
      {
        in.clear();
      }
    }
  }

  void printSeqName(const List< pair_t >& data)
  {
    bool first = true;

    LCIter< pair_t > it = data.begin();
    LCIter< pair_t > end = data.end();

    while (it != end) {
      if (!first) {
        std::cout << ' ';
      }

      std::cout << (*it).first;

      first = false;
      ++it;
    }
    std::cout << '\n';
  }

  size_t printOneRow(List< pair_t >& data)
  {
    LIter< pair_t > it = data.begin();
    LIter< pair_t > end = data.end();

    bool first = true;
    bool overflow = false;

    size_t sum = 0;

    while (it != end) {
      if (!((*it).second.empty())) {
        size_t value = (*it).second.front();

        if (!first) {
          std::cout << ' ';
        }

        std::cout << value;

        if (!overflow) {
          if (sum > std::numeric_limits< size_t >::max() - value) {
            overflow = true;
          }
          else {
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

  bool hasNum(const List< pair_t >& data)
{
    LCIter< pair_t > it = data.begin();
    LCIter< pair_t > end = data.end();

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

  void printAll(List< pair_t >& data, List< size_t >& sums)
  {
    while (hasNum(data))
    {
      size_t sum = printOneRow(data);
      sums.pushBack(sum);
    }
  }

  void printSum(const List< size_t >& sums)
  {
    if (sums.empty())
    {
      std::cout << 0;
    }

    bool first = true;

    LCIter< size_t > it = sums.begin();
    LCIter< size_t > end = sums.end();

    while (it != end) {
      if (!first) {
        std::cout << ' ';
      }

      std::cout << *it;

      first = false;
      it++;
    }

    std::cout << '\n';
  }
}

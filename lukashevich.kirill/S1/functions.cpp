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

  void printSum(const List< size_t >& sums)
  {
    if (sums.empty())
    {
      std::cout << 0 << '\n';
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

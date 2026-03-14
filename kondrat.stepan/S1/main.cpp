#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"

namespace kondrat
{
  using pairN = std::pair< std::string, kondrat::List< size_t > >;

  void getData(std::istream & in, kondrat::List< pairN > & res)
  {
    std::string name;

    while (in >> name)
    {
      kondrat::List< size_t > nums;
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

  void printNames(const kondrat::List< pairN > & data)
  {
    bool first = true;

    kondrat::LCIter< pairN > it = data.begin();
    kondrat::LCIter< pairN > end = data.end();

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
}

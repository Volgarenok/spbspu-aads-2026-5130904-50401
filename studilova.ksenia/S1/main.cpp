#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include"../common/list.hpp"

int main()
{
  studilova::List< std::pair< std::string, studilova::List< size_t > > > list;

  std::string name;
  while(std::cin >> name)
  {
    studilova::List< size_t > seq;
    size_t value = 0;

    while (std::cin >> value)
    {
      seq.pushBack(value);
    }

    list.pushBack({ name, seq });

    if (std::cin.bad())
    {
      return 1;
    }
    std::cin.clear();
  }

  if (list.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  auto it = list.begin();
  for (size_t i = 0; i < list.size(); ++i, ++it)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << (*it).first;
  }
  std::cout << "\n";

  size_t maxLen = 0;
  it = list.begin();
  for (size_t i = 0; i < list.size(); ++i, ++it)
  {
    size_t currLen = (*it).second.size();
    if (currLen > maxLen)
    {
      maxLen = currLen;
    }
  }

  if (maxLen == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  studilova::List< size_t > sums;
  for (size_t i = 0; i < maxLen; ++i)
  {
    sums.pushBack(0);
  }

  const size_t MAX = std::numeric_limits< size_t >::max();

  auto sumIt = sums.begin();
  for(size_t row = 0; row < maxLen; ++row, ++sumIt)
  {
    bool first = true;
    it = list.begin();

    for (size_t i = 0; i < list.size(); ++i, ++it)
    {
      auto& seq = (*it).second;

      if (row < seq.size())
      {
        auto sit = seq.begin();
        for (size_t j = 0; j < row; ++j)
        {
          ++sit;
        }

        if(!first)
        {
          std::cout << " ";
        }
        std::cout << *sit;

        if(MAX - *sit < *sumIt)
        {
          std::cout << "\n";
          std::cerr << "Overflow\n";
          return 1;
        }
        *sumIt += *sit;
        first = false;
      }
    }
    std::cout << "\n";
  }

  sumIt = sums.begin();
  for (size_t i = 0; i < sums.size(); ++i, ++sumIt)
  {
    if (i != 0)
    {
      std::cout << " ";
    }
    std::cout << *sumIt;
  }
  std::cout << "\n";
  return 0;
}

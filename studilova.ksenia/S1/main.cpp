#include <iostream>
#include <string>
#include <limits>
#include"list.hpp"

int main()
{
  studilova::List< std::pair< std::string, studilova::List< size_t > > > sequences;

  std::string name;
  while(std::cin >> name)
  {
    studilova::List< size_t > numbers;
    size_t value = 0;

    while (std::cin >> value)
    {
      numbers.pushBack(value);
    }

    std::cin.clear();
    sequences.pushBack({ name, numbers });
  }

  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  size_t maxLen = 0;
  auto it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    size_t len = (*it).second.size();
    if (len > maxLen)
    {
      maxLen = len;
    }
    ++it;
  }

  const size_t MAX = std::numeric_limits< size_t >::max();
  studilova::List< size_t > sums;
  for (size_t i = 0; i < maxLen; ++i)
  {
    sums.pushBack(0);
  }

  for(size_t row = 0; row < maxLen; ++row)
  {
    size_t sum = 0;
    it = sequences.begin();

    for (size_t i = 0; i < sequences.size(); ++i)
    {
      auto& seq = (*it).second;

      if (row < seq.size())
      {
        auto numIt = seq.begin();
        for (size_t j = 0; j < row; ++j)
        {
          ++numIt;
        }
        size_t value = *numIt;

        if(MAX - value < sum)
        {
          std::cerr << "Overflow\n";
          return 1;
        }
        sum += value;
      }
      ++it;
    }

    auto sumIt = sums.begin();
    for (size_t k = 0; k < row; ++k)
    {
      ++sumIt;
    }
    *sumIt = sum;
  }

  it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << (*it).first;
    ++it;
  }
  std::cout << "\n";


  if (maxLen == 0)
  {
   std::cout << "0\n";
   return 0;
  }

  for (size_t row = 0; row < maxLen; ++row)
  {
    bool needSpace = false;
    it = sequences.begin();

    for (size_t i = 0; i < sequences.size(); ++i)
    {
      auto& seq = (*it).second;
      if (row < seq.size())
      {
        auto numIt = seq.begin();
        for (size_t j = 0; j < row; ++j)
        {
          ++numIt;
        }

        if (needSpace)
        {
          std::cout << " ";
        }
        std::cout << *numIt;
        needSpace = true;
      }
      ++it;
    }
    std::cout << "\n";
  }

  auto sumIt = sums.begin();
  for (size_t i = 0; i < sums.size(); ++i)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << *sumIt;
    ++sumIt;
  }
  std::cout << "\n";
  return 0;
}

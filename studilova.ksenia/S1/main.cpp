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

    while(std::cin >> value)
    {
      numbers.pushBack(value);
    }
    sequences.pushBack({ name, numbers });

    if (std::cin.bad())
    {
      return 1;
    }
    std::cin.clear();
  }

  if (sequences.empty())
  {
    std::cout << "0" << "\n";
    return 0;
  }

  auto it = sequences.begin();
  for (size_t i = 0; i <= sequences.size(); ++i)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << (*it).first;
    ++it;
  }
  std::cout << "\n";

  size_t maxLen = 0;
  it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    size_t len = (*it).second.size();
    if (len > maxLen)
    {
      maxLen = len;
    }
    ++it;
  }

  studilova::List< studilova::LIter< size_t > > iters;

  it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    iters.pushBack((*it).second.begin());
    ++it;
  }

  size_t MAX = std::numeric_limits< size_t >::max();

  for(size_t row = 0; row < maxLen; ++row)
  {
    size_t sum = 0;

    auto seqIt = sequences.begin();
    auto iterIt = iters.begin();

    bool first = true;

    for (size_t i = 0; i < sequences.size(); ++i)
    {
      auto& seq = (*seqIt).second;

      if (row < seq.size())
      {
        size_t value = **iterIt;
        if(!first)
        {
          std::cout << " ";
        }
        std::cout << value;
        if(MAX - value < sum)
        {
          std::cerr << "Overflow" << "\n";
          return 1;
        }
        sum += value;
        ++(*iterIt);
        first = false;
      }
      ++seqIt;
      ++iterIt;
    }
    std::cout << "\n";
  }

  return 0;
}

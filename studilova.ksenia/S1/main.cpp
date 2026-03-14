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

  if (maxLen == 0)
  {
    auto it2 = sequences.begin();
    for (size_t i = 0; i < sequences.size(); ++i)
    {
      if (i > 0)
      {
        std::cout << " ";
      }
      std::cout << (*it2).first;
      ++it2;
    }
    std::cout << "0" << "\n";
    return 0;
  }

  studilova::List< studilova::LIter< size_t > > iters;

  it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    iters.pushBack((*it).second.begin());
    ++it;
  }

  const size_t MAX = std::numeric_limits< size_t >::max();

  studilova::List< studilova::List< size_t > > rows;
  studilova::List< size_t > sums;

  for(size_t row = 0; row < maxLen; ++row)
  {
    size_t sum = 0;

    auto seqIt = sequences.begin();
    auto iterIt = iters.begin();

    studilova::List< size_t> rowValues;

    for (size_t i = 0; i < sequences.size(); ++i)
    {
      auto& seq = (*seqIt).second;

      if (row < seq.size())
      {
        size_t value = **iterIt;

        if(MAX - value < sum)
        {
          std::cerr << "Overflow" << "\n";
          return 1;
        }
        sum += value;
        rowValues.pushBack(value);

        ++(*iterIt);
      }
      ++seqIt;
      ++iterIt;
    }
    rows.pushBack(rowValues);
    sums.pushBack(sum);
  }

  auto nameIt = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << (*nameIt).first;
    ++nameIt;
  }

  auto rowIt = rows.begin();
  for (size_t i = 0; i < rows.size(); ++i)
  {
    auto valIt = (*rowIt).begin();
    for (size_t j = 0; j < (*rowIt).size(); ++j)
    {
      if (j > 0)
      {
        std::cout << " ";
      }
      std::cout << *valIt;
      ++valIt;
    }
    std::cout << "\n";
    ++rowIt;
  }

  auto sIt = sums.begin();
  for (size_t i = 0; i < sums.size(); ++i)
  {
    if (i > 0)
    {
      std::cout << " ";
    }
    std::cout << *sIt;
    ++sIt;
  }
  std::cout << "\n";
  return 0;
}

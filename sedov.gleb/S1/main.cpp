#include <iostream>
#include <cstddef>
#include <utility>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "list.hpp"

int main()
{
  sedov::List< std::pair< std::string, sedov::List< size_t > > > sequences;
  std::string str;

  while (std::cin >> str)
  {
    sedov::List< size_t > numbers;
    size_t num = 0;
    while (std::cin >> num)
    {
      numbers.pushBack(num);
    }
    std::cin.clear();
    sequences.pushBack(std::make_pair(str, std::move(numbers)));
  }

  if (sequences.begin() == sequences.end())
  {
    std::cout << "0\n";
    return 0;
  }

  auto seq_it = sequences.begin();
  std::cout << seq_it->first;
  ++seq_it;
  while (seq_it != sequences.end())
  {
    std::cout << " " << seq_it->first;
    ++seq_it;
  }
  std::cout << "\n";

  size_t maxLen = 0;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t len = 0;
    for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
    {
      ++len;
    }
    maxLen = std::max(maxLen, len);
  }

  sedov::List< sedov::List< size_t > > transpose;
  bool hasNum = false;
  for (size_t pos = 0; pos < maxLen; ++pos)
  {
    sedov::List< size_t > newSeq;
    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      auto nit = sit->second.cbegin();
      size_t cur = 0;
      while (cur < pos && nit != sit->second.cend())
      {
        ++cur;
        ++nit;
      }
      if (nit != sit->second.cend())
      {
        newSeq.pushBack(*nit);
        hasNum = true;
      }
    }
    if (newSeq.begin() != newSeq.end())
    {
      transpose.pushBack(std::move(newSeq));
    }
  }

  if (hasNum)
  {
    for (auto tit = transpose.begin(); tit != transpose.end(); ++tit)
    {
      if (tit->cbegin() != tit->cend())
      {
        auto nit = tit->cbegin();
        std::cout << *nit;
        ++nit;
        while (nit != tit->cend())
        {
          std::cout << " " << *nit;
          ++nit;
        }
      }
      std::cout << "\n";
    }

    sedov::List< size_t > sums;
    for (auto tit = transpose.cbegin(); tit != transpose.cend(); ++tit)
    {
      size_t sum = 0;
      for (auto nit = tit->cbegin(); nit != tit->cend(); ++nit)
      {
        if (sum > sedov::MAX - *nit)
        {
          std::cerr << "Sum overflow\n";
          return 1;
        }
        sum += *nit;
      }
      sums.pushBack(sum);
    }

    if (sums.size())
    {
      auto sit = sums.cbegin();
      std::cout << *sit;
      ++sit;
      while (sit != sums.cend())
      {
        std::cout << " " << *sit;
        ++sit;
      }
      std::cout << "\n";
    }
  }
  else
  {
    std::cout << "0\n";
  }
  return 0;
}

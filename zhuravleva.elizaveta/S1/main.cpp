#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include "list.hpp"

int main()
{
  zhuravleva::List< std::pair<std::string, zhuravleva::List< size_t > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    zhuravleva::List< size_t > numbers;
    size_t value = 0;
    while (std::cin >> value)
    {
      numbers.addEnd(value);
    }
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    sequences.addEnd(std::make_pair(name, numbers));
    if (std::cin.bad())
    {
      std::cerr << "bad input\n";
      return 1;
    }
    std::cin.clear();
  }

  if (!std::cin.eof())
  {
    std::cerr << "bad input\n";
    return 1;
  }

  if (sequences.empty())
  {
    std::cout << 0 << "\n";
    return 0;
  }

  auto sit = sequences.begin();
  std::cout << sit->first;
  ++sit;
  while (sit != sequences.end())
  {
    std::cout << " " << sit->first;
    ++sit;
  }
  std::cout << "\n";

  size_t maxLen = 0;
  auto it = sequences.cbegin();
  while (it != sequences.cend())
  {
    size_t len = 0;
    auto nit = it->second.cbegin();
    while (nit != it->second.cend())
    {
      ++len;
      ++nit;
    }
    if (len > maxLen)
    {
      maxLen = len;
    }
    ++it;
  }

  zhuravleva::List< zhuravleva::List< size_t > > transposed;
  auto tpos = transposed.beforeStart();

  for (size_t column = 0; column < maxLen; ++column)
  {
    zhuravleva::List< size_t > newRow;
    auto insertPos = newRow.beforeStart();
    auto sit = sequences.cbegin();
    while (sit != sequences.cend())
    {
      auto nit = sit->second.cbegin();
      size_t index = 0;
      while ((index < column) && (nit != sit->second.cend()))
      {
        ++nit;
        ++index;
      }
      if (nit != sit->second.cend())
      {
        insertPos = newRow.addAfter(insertPos, *nit);
      }
      ++sit;
    }
    if (!newRow.empty())
    {
      tpos = transposed.addAfter(tpos, newRow);
    }
  }

  auto tit = transposed.cbegin();
  while (tit != transposed.cend())
  {
    auto nit = tit->cbegin();
    if (nit != tit->cend())
    {
      std::cout << *nit;
      ++nit;
    }
    while (nit != tit->cend())
    {
      std::cout << " " << *nit;
      ++nit;
    }
    std::cout << "\n";
    ++tit;
  }

  zhuravleva::List< size_t > sums;
  auto spos = sums.beforeStart();
  auto tit2 = transposed.cbegin();

  while (tit2 != transposed.cend())
  {
    size_t sum = 0;
    auto nit = tit2->cbegin();
    while (nit != tit2->cend())
    {
      if (sum > std::numeric_limits< size_t >::max() - *nit)
      {
        std::cerr << "overflow\n";
        return 1;
      }

      sum += *nit;
      ++nit;
    }

    spos = sums.addAfter(spos, sum);
    ++tit2;
  }

  if (transposed.empty())
  {
    sums.addEnd(0);
  }
  auto sit2 = sums.begin();
  if (sit2 != sums.end())
  {
    std::cout << *sit2;
    ++sit2;
  }

  while (sit2 != sums.end())
  {
    std::cout << " " << *sit2;
    ++sit2;
  }
  std::cout << "\n";
  return 0;
}

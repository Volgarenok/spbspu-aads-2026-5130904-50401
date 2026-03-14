#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include "list.hpp"


int main()
{
  zhuravleva::List< std::pair<std::string, zhuravleva::List<size_t > > > sequences;
  std::string name;

  while(std::cin >> name)
  {
    zhuravleva::List< size_t > numbers;
    size_t value = 0;
    while(std::cin >> value)
    {
      numbers.addEnd(value);
    }
    if(std::cin.fail())
    {
      std::cin.clear();
    }
    sequences.addEnd(std::make_pair(name, numbers));
    if (std::cin.bad()) {
      std::cerr << "bad input\n";
      return 1;
    }
    std::cin.clear();
  }

  if (!std::cin.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }

  if (sequences.empty()) {
    std::cout << 0 << "\n";
    return 0;
  }

  auto sit = sequences.begin();
  while(sit != sequences.end())
  {
    std::cout << (*sit).first;
    sit = sit.next();
    if(sit != sequences.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  size_t maxLen = 0;
  auto it = sequences.cbegin();
  while(it != sequences.cend())
  {
    size_t len = 0;
    auto nit = (*it).second.cbegin();
    while(nit != (*it).second.cend())
    {
      len++;
      nit = nit.next();
    }
    if(len > maxLen)
    {
      maxLen = len;
    }
    it = it.next();
  }

  zhuravleva::List< zhuravleva::List<size_t> > transposed;
  auto tpos = transposed.beforeStart();

  for(size_t column = 0; column < maxLen; column++)
  {
    zhuravleva::List<size_t> newRow;
    auto insertPos = newRow.beforeStart();
    auto sit = sequences.cbegin();

    while(sit != sequences.cend())
    {
      auto nit = (*sit).second.cbegin();
      size_t index = 0;
      while(index < column && nit != (*sit).second.cend())
      {
        nit = nit.next();
        index++;
      }
      if(nit != (*sit).second.cend())
      {
        insertPos = newRow.addAfter(insertPos, *nit);
      }
      sit = sit.next();
    }
    if(!newRow.empty())
    {
      tpos = transposed.addAfter(tpos, newRow);
    }
  }

 auto tit = transposed.cbegin();
  while(tit != transposed.cend())
  {
    auto nit = (*tit).cbegin();
    while(nit != (*tit).cend())
    {
      std::cout << *nit;
      nit = nit.next();
      if(nit != (*tit).cend())
      {
        std::cout << " ";
      }
    }
    std::cout << "\n";
    tit = tit.next();
  }

  zhuravleva::List<size_t> sums;
  auto spos = sums.beforeStart();
  auto tit2 = transposed.cbegin();

  while(tit2 != transposed.cend())
  {
    size_t sum = 0;
    auto nit = (*tit2).cbegin();
    while(nit != (*tit2).cend())
    {
      if(sum > std::numeric_limits<size_t>::max() - *nit)
      {
        std::cerr << "overflow\n";
        return 1;
      }

      sum += *nit;
      nit = nit.next();
    }

    spos = sums.addAfter(spos, sum);
    tit2 = tit2.next();
  }

  auto sit2 = sums.begin();
  while(sit2 != sums.end())
  {
    std::cout << *sit2;
    sit2 = sit2.next();
    if(sit2 != sums.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  return 0;

}

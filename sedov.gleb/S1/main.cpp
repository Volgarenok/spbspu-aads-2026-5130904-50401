#include <iostream>
#include <cstddef>
#include <utility>
#include <string>
#include <stdexcept>
#include "list.hpp"

int main()
{
  sedov::List< std::pair< std::string, sedov::List< size_t > > > sequnces;
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
    sequnces.pushBack(std::make_pair(str, std::move(numbers)));
  }

  if (sequnces.begin() == sequnces.end())
  {
    std::cout << "0\n";
    return 0;
  }

  auto seq_it = sequnces.begin();
  std::cout << seq_it->first;
  ++seq_it;
  while (seq_it != sequnces.end())
  {
    std::cout << " " << seq_it->first;
    ++seq_it;
  }
  std::cout << "\n";

  size_t maxLen = 0;
  for (auto it = sequnces.cbegin(); it != sequnces.cend(); ++it)
  {
    size_t len = 0;
    for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
    {
      ++len;
    }
    if (len > maxLen)
    {
      maxLen = len;
    }
  }

  sedov::List< sedov::List< size_t > > transpose;
  bool hasNum = false;
  for (size_t pos = 0; pos < maxLen; ++pos)
  {
    sedov::List< size_t > newSeq;
    for (auto sit = sequnces.cbegin(); sit != sequnces.cend(); ++sit)
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
}

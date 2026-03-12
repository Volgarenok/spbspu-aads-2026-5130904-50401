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
    auto pos = numbers.beforeStart();
    while(std::cin >> value)
    {
      pos = numbers.addAfter(pos, value);
    }
    std::cin.clear();
    sequences.AddStart(std::make_pair(name, numbers));
  }

  if(sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  auto sit = sequences.begin();

  while(sit.hasNext())
  {
    std::cout << (*sit).first;

    sit = sit.next();

    if(sit.hasNext())
    {
      std::cout << " ";
    }
  }

  std::cout << "\n";

  size_t maxLen = 0;

  auto it = sequences.cbegin();

  while(it.hasNext())
  {
    size_t len = 0;

    auto nit = (*it).second.cbegin();

    while(nit.hasNext())
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

  return 0;
}

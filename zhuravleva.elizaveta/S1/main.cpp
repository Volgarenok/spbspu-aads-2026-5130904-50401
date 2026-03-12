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

  return 0;
}

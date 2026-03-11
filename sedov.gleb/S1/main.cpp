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
}

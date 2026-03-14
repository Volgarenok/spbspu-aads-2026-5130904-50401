#include <iostream>
#include <string>
#include"list.hpp"

int main()
{
  studilova::List< std::pair< std::string, studilova::List<int> > > sequences;

  std::string name;

  while(std::cin >> name)
  {
    studilova::List< int > numbers;

    while(true)
    {
      int value;

      if (!(std::cin >> value))
      {
        std::cin.clear();
        break;
      }
      numbers.push_back(value);
    }
    sequences.push_back({ name, numbers });
  }

  if (sequences.empty())
  {
    std::cout << 0 << "\n";
    return 0;
  }

  auto it = sequences.begin();
  for (size_t i = 0; i <= sequences.size(); ++i)
  {
    std::cout << (*it).first << " ";
  }
  std::cout << "\n";

  size_t max_len = 0;
  it = sequences.begin();
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    size_t len = (*it).second.size();
    if (len > max_len)
    {
      max_len = len;
    }
    ++it;
  }

  return 0;
}

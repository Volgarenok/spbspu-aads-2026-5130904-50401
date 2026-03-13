#include <iostream>
#include "list.hpp"
#include <string>

int main()
{
  zharov::List< std::pair< std::string, zharov::List< int > > > data;
  std::string name;
  while (std::cin >> name) {
    int num;
    zharov::List< int > nums;
    while (std::cin >> num) {
      nums.pushBack(num);
    }
    data.pushBack(make_pair(name, nums));
    std::cin.clear();
  }

  std::cout << data.size() << '\n';
  auto it = data.constBegin();
  for (size_t i = 0; i < data.size(); ++i) {
    std::cout << it->first << " ";
    auto num_it = it->second.constBegin();
    while (num_it != it->second.constEnd()) {
      std::cout << *num_it << " ";
      ++num_it;
    }
    ++it;
    std::cout << "\n";
  }
}

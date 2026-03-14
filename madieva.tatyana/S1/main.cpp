#include <iostream>
#include <string>
#include <utility>
#include"list.hpp"

void read(madieva::List<std::pair<std::string, madieva::List<int>>> & list)
{
  std::string name;
  while (std::cin >> name) {
    madieva::List<int> numbers;
    int num;
    while (std::cin >> num) {
      numbers.push_back(num);
    }
    std::pair<std::string, madieva::List<int>> p;
    p = {name, numbers};
    list.push_back(p);
    if (std::cin.eof()) {
      return;
    } else if (std::cin.fail()) {
      std::cin.clear();
    }
  }
}

void transposition(madieva::List<madieva::List<int>> & t_list,
  const madieva::List<std::pair<std::string, madieva::List<int>>> & list)
{
  size_t max_size = 0;
  auto it = list.begin();
  for(size_t i = 0; i < list.size(); ++i) {
    if (max_size < (*it).second.size()) {
      max_size = (*it).second.size();
    }
    ++it;
  }
}

int main()
{
  namespace mad =  madieva;
  mad::List<std::pair<std::string, mad::List<int>>> list;
  read(list);
  mad::List<mad::List<int>> t_list;

}
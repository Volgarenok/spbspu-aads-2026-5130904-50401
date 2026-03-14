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

void w_row(const madieva::List<std::pair<std::string, madieva::List<int>>> & list,
  madieva::List< int > & l_numbers, size_t i)
{
  auto it = list.begin();
  size_t size = list.size();
  for(size_t j = 0; j < size; ++j) {
    if ((*it).second.size() > i) {
      madieva::LCIter< int > it_num = (*it).second.begin();
      for(size_t k =  0; k < i; ++k) {
        ++it_num;
      }
      l_numbers.push_back((*it_num));
    }
    ++it;
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
  for (size_t i = 0; i < max_size; ++ i) {
    madieva::List< int > l_numbers;
    w_row(list, l_numbers, i);
    t_list.push_back(l_numbers);
  }
}

void sum_num(const madieva::List< madieva::List< int >> & t_list, madieva::List< int > & sum)
{
  madieva::LCIter<madieva::List < int >> l_it = t_list.begin();
  for(size_t i = 0; i < t_list.size(); ++i) {
    int s = 0;
    madieva::LCIter<int> n_it = (*l_it).begin();
    for (size_t j = 0; j < (*l_it).size(); ++j) {
      s += (*n_it);
      ++n_it;
    }
    sum.push_back(s);
    ++l_it;
  }
}

void print(const madieva::List< madieva::List< int >> & t_list, const madieva::List<std::pair<std::string, madieva::List<int>>> & list, const madieva::List< int > & sum)
{
  madieva::LCIter<std::pair<std::string, madieva::List<int>>> it = list.begin();
  std::cout << (*it).first;
  ++it;
  for(size_t i = 1; i < list.size(); ++i) {
    std::cout << " " << (*it).first;
    ++it;
  }
  std::cout << "\n";

  for (size_t i = 0; i < t_list.size(); ++i) {
    madieva::LCIter< int > it_num = (*it).second.begin();
    std::cout << (*it_num);
    ++it_num;
    for(size_t j = 1; j < (*it).second.size();++j) {
      std::cout << " " << (*it_num);
    }
    std::cout << "\n";
  }

  madieva::LCIter< int > s_it = sum.begin();
  std::cout << (*s_it);
  ++s_it;
  for(size_t i = 1; i < sum.size(); ++i) {
    std::cout << " " << (*s_it);
  }
  std::cout << "\n";
}

int main()
{
  namespace mad =  madieva;
  mad::List<std::pair<std::string, mad::List<int>>> list;
  read(list);
  mad::List<mad::List<int>> t_list;
  transposition(t_list, list);
  mad::List< int > sum;
  print(t_list, list, sum);
  return 0;
}
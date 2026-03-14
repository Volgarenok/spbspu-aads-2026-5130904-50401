#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include"list.hpp"
namespace madieva{
  void read(List<std::pair<std::string,
    List<int>>> & list)
  {
    std::string name;
    while (std::cin >> name) {
      List<int> numbers;
      int num;
      while (std::cin >> num) {
        numbers.push_back(num);
      }
      std::pair<std::string, List<int>> p;
      p = {name, numbers};
      list.push_back(p);
      if (std::cin.eof()) {
        return;
      } else if (std::cin.fail()) {
        std::cin.clear();
      }
    }
  }

  void w_row(const List<std::pair<std::string,
    List<int>>> & list,
    List< int > & l_numbers,
    size_t i)
  {
    auto it = list.begin();
    size_t size = list.size();
    for(size_t j = 0; j < size; ++j) {
      if ((*it).second.size() > i) {
        LCIter< int > it_num = (*it).second.begin();
        for(size_t k =  0; k < i; ++k) {
          ++it_num;
        }
        l_numbers.push_back((*it_num));
      }
      ++it;
    }
  }

  void transposition(List<List<int>> & t_list,
    const List<std::pair<std::string, List<int>>> & list)
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
      List< int > l_numbers;
      w_row(list, l_numbers, i);
      t_list.push_back(l_numbers);
    }
  }

  void sum_num(const List< List< int >> & t_list,
    List< int > & sum)
  {
    using lim_int = std::numeric_limits< int >;
    const int max_int = lim_int::max();
    LCIter<List < int >> l_it = t_list.begin();
    for(size_t i = 0; i < t_list.size(); ++i) {
      int s = 0;
      LCIter<int> n_it = (*l_it).begin();
      for (size_t j = 0; j < (*l_it).size(); ++j) {
          if((*n_it) < max_int - s) {
          s += (*n_it);
          ++n_it;
        } else {
          throw std::overflow_error("Error");
        }
      }
      sum.push_back(s);
      ++l_it;
    }
  }

  void print(const List< List< int >> & t_list,
    const List<std::pair<std::string, List<int>>> & list,
    const List< int > & sum)
  {
    LCIter<std::pair<std::string, List<int>>> p_it = list.begin();
    std::cout << (*p_it).first;
    ++p_it;
    for(size_t i = 1; i < list.size(); ++i) {
      std::cout << " " << (*p_it).first;
      ++p_it;
    }
    std::cout << "\n";

    LCIter< List< int >> l_it = t_list.begin();
    for (size_t i = 0; i < t_list.size(); ++i) {
      LCIter< int > it_num = (*l_it).begin();
      std::cout << (*it_num);
      ++it_num;
      for(size_t j = 1; j < (*l_it).size();++j) {
        std::cout << " " << (*it_num);
        ++it_num;
      }
      ++l_it;
      std::cout << "\n";
    }

    LCIter< int > s_it = sum.begin();
    std::cout << (*s_it);
    ++s_it;
    for(size_t i = 1; i < sum.size(); ++i) {
      std::cout << " " << (*s_it);
      ++s_it;
    }
    std::cout << "\n";
  }
}

int main()
{
  namespace mad =  madieva;
  mad::List<std::pair<std::string, mad::List<int>>> list;
  mad::read(list);
  if (list.size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  mad::List<mad::List<int>> t_list;
  mad::transposition(t_list, list);
  mad::List< int > sum;
  try {
    mad::sum_num(t_list, sum);
  } catch (const std::overflow_error & e) {
    std::cerr << e.what() << "\n";
    t_list.clear();
    list.clear();
    sum.clear();
    return 1;
  }
  mad::print(t_list, list, sum);
  t_list.clear();
  list.clear();
  sum.clear();
  return 0;
}

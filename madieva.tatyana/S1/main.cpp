#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include"list.hpp"
namespace madieva{
  void read(List< std::pair< std::string,
    List< size_t >>> & list)
  {
    std::string name;
    while (std::cin >> name) {
      List<size_t> numbers;
      size_t num = 0;
      while (std::cin >> num) {
        numbers.push_back(num);
      }
      std::pair<std::string, List<size_t>> p;
      p = {name, numbers};
      list.push_back(p);
      if (std::cin.eof()) {
        return;
      } else if (std::cin.bad()) {
        throw std::overflow_error("overflow");
      } else if (std::cin.fail()) {
        std::cin.clear();
      }
    }
  }

  void w_row(const List<std::pair<std::string,
    List<size_t>>> & list,
    List< size_t > & l_numbers,
    size_t i)
  {
    auto it = list.begin();
    size_t size = list.size();
    for(size_t j = 0; j < size; ++j) {
      if ((*it).second.size() > i) {
        LCIter< size_t > it_num = (*it).second.begin();
        for(size_t k =  0; k < i; ++k) {
          ++it_num;
        }
        l_numbers.push_back((*it_num));
      }
      ++it;
    }
  }

  void transposition(List<List<size_t>> & t_list,
    const List<std::pair<std::string, List<size_t>>> & list)
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
      List< size_t > l_numbers;
      w_row(list, l_numbers, i);
      t_list.push_back(l_numbers);
    }
  }

  void sum_num(const List< List< size_t >> & t_list,
    List< size_t > & sum)
  {
    using lim_size_t = std::numeric_limits< size_t >;
    const size_t max_size_t = lim_size_t::max();
    LCIter<List < size_t >> l_it = t_list.begin();
    for(size_t i = 0; i < t_list.size(); ++i) {
      size_t s = 0;
      LCIter<size_t> n_it = (*l_it).begin();
      for (size_t j = 0; j < (*l_it).size(); ++j) {
          if((*n_it) < max_size_t - s) {
          s += (*n_it);
          ++n_it;
        } else {
          throw std::overflow_error("overflow");
        }
      }
      sum.push_back(s);
      ++l_it;
    }
  }

  void print_str(const List<std::pair<std::string, List<size_t>>> & list)
  {
    LCIter<std::pair<std::string, List<size_t>>> p_it = list.begin();
    std::cout << (*p_it).first;
    ++p_it;
    for(size_t i = 1; i < list.size(); ++i) {
      std::cout << " " << (*p_it).first;
      ++p_it;
    }
    std::cout << "\n";
  }

  void print_list(const List< List< size_t >> & t_list)
  {
    if (t_list.size() == 0) {
      std::cout << "0\n";
      return;
    }

    LCIter< List< size_t >> l_it = t_list.begin();
    for (size_t i = 0; i < t_list.size(); ++i) {
      LCIter< size_t > it_num = (*l_it).begin();
      std::cout << (*it_num);
      ++it_num;
      for(size_t j = 1; j < (*l_it).size();++j) {
        std::cout << " " << (*it_num);
        ++it_num;
      }
      ++l_it;
      std::cout << "\n";
    }
  }

  void  print_sum(const List< size_t > & sum)
  {
    LCIter< size_t > s_it = sum.begin();
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
  mad::List<std::pair<std::string, mad::List<size_t>>> list;
  try {
    mad::read(list);
  } catch (const std::overflow_error & e) {
    std::cerr << e.what() << "\n";
    list.clear();
    return 1;
  }
  if (list.size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  mad::List<mad::List<size_t>> t_list;
  mad::transposition(t_list, list);
  if (t_list.size() == 0) {
    mad::print_str(list);
    std::cout << "0\n";
    return 0;
  }
  mad::List< size_t > sum;
  try {
    mad::sum_num(t_list, sum);
  } catch (const std::overflow_error & e) {
    mad::print_str(list);
    mad::print_list(t_list);
    std::cerr << e.what() << "\n";
    t_list.clear();
    list.clear();
    sum.clear();
    return 1;
  }
  mad::print_str(list);
  mad::print_list(t_list);
  mad::print_sum(sum);
  t_list.clear();
  list.clear();
  sum.clear();
  return 0;
}

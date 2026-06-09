#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include "list.hpp"

namespace madieva
{
  namespace
  {
    void read(List< std::pair< std::string,
      List< size_t > > > & list)
    {
      std::string name;
      while (std::cin >> name) {
        List< size_t > numbers;
        size_t num = 0;
        while (std::cin >> num) {
          numbers.pushBack(num);
        }
        std::pair<std::string, List<size_t>> p;
        p = {name, numbers};
        list.pushBack(p);
        if (std::cin.eof()) {
          return;
        } else if (std::cin.bad()) {
          throw std::ios_base::failure("I/O stream failure");
        }
        std::cin.clear();
      }
    }

    void writeRow(const List< std::pair< std::string,
      List< size_t > > > & list,
      List< size_t > & l_numbers,
      size_t i)
    {
      auto it = list.begin();
      size_t size = list.size();
      for (size_t j = 0; j < size; ++j) {
        if (it->second.size() > i) {
          LCIter< size_t > it_num = it->second.begin();
          for (size_t k =  0; k < i; ++k) {
            ++it_num;
          }
          l_numbers.pushBack(*it_num);
        }
        ++it;
      }
    }

    void transpose(List< List< size_t > > & t_list,
      const List<std::pair< std::string, List< size_t > > > & list)
    {
      size_t max_size = 0;
      auto it = list.begin();
      for (size_t i = 0; i < list.size(); ++i) {
        if (max_size < it->second.size()) {
          max_size = it->second.size();
        }
        ++it;
      }
      for (size_t i = 0; i < max_size; ++ i) {
        List< size_t > l_numbers;
        writeRow(list, l_numbers, i);
        t_list.pushBack(l_numbers);
      }
    }

    void calculateSums(const List< List< size_t > > & t_list,
      List< size_t > & sum)
    {
      using lim_size_t = std::numeric_limits< size_t >;
      const size_t max_size_t = lim_size_t::max();
      LCIter<List < size_t >> l_it = t_list.begin();
      for (; l_it != t_list.end(); ++l_it) {
        size_t s = 0;
        LCIter< size_t > n_it = l_it->begin();
        for (; n_it != l_it->end(); ++n_it) {
            if ((*n_it) < max_size_t - s) {
            s += *n_it;
          } else {
            throw std::overflow_error("overflow");
          }
        }
        sum.pushBack(s);
      }
    }

    void printStrings(const List< std::pair< std::string, List< size_t > > > & list)
    {
      LCIter< std::pair< std::string, List< size_t > > > p_it = list.begin();
      if (p_it != list.end()) {
        std::cout << p_it->first;
        ++p_it;
        for (; p_it != list.end(); ++p_it) {
          std::cout << " " << p_it->first;
        }
      }
    }

    void printMatrix(const List< List< size_t > > & t_list)
    {
      if (t_list.empty()) {
        std::cout << "0\n";
        return;
      }

      LCIter< List< size_t > > l_it = t_list.begin();
      for (; l_it != t_list.end(); ++l_it) {
        LCIter< size_t > it_num = l_it->begin();
        if (it_num != l_it->end()) {
          std::cout << *it_num;
          ++it_num;
          for (; it_num != l_it->end(); ++it_num) {
            std::cout << " " << *it_num;
          }
        }
        std::cout << "\n";
      }
    }

    void printSums(const List< size_t > & sum)
    {
      LCIter< size_t > s_it = sum.begin();
      if (s_it != sum.end()) {
        std::cout << (*s_it);
        ++s_it;
        for (; s_it != sum.end(); ++s_it) {
          std::cout << " " << (*s_it);
        }
      }
    }
  }
}

int main()
{
  namespace mad =  madieva;
  mad::List< std::pair< std::string, mad::List< size_t > > > list;
  try {
    mad::read(list);
  } catch (const std::ios_base::failure & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  if (list.empty()) {
    std::cout << "0\n";
    return 0;
  }
  mad::List< mad::List< size_t > > t_list;
  mad::transpose(t_list, list);
  if (t_list.empty()) {
    mad::printStrings(list);
    std::cout << "\n";
    std::cout << "0\n";
    return 0;
  }
  mad::List< size_t > sum;
  try {
    mad::calculateSums(t_list, sum);
  } catch (const std::overflow_error & e) {
    mad::printStrings(list);
    std::cout << "\n";
    mad::printMatrix(t_list);
    std::cerr << e.what() << "\n";
    return 1;
  }
  mad::printStrings(list);
  std::cout << "\n";
  mad::printMatrix(t_list);
  mad::printSums(sum);
  std::cout << "\n";
  return 0;
}

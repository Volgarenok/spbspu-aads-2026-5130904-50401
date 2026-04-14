#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include <stdexcept>
#include "list.hpp"

int sum(int a, int b) {
  if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
      (b < 0 && a < std::numeric_limits<int>::min() - b)) {
    throw std::overflow_error("overflow");
  }
  return a + b;
}

int main() {
  petrov::List<std::pair<std::string, petrov::List<int>>> list_for_sol;
  std::string s;
  bool is_ovf = false;

  try {
    while (std::cin >> s) {
      petrov::List<int> count_nums;
      while (std::cin.peek() == ' ') std::cin.ignore();
      
      while (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
        std::string raw_val;
        if (!(std::cin >> raw_val)) break;

        try {
          if (raw_val.length() > 11) {
             is_ovf = true;
          } else {
             long long v = std::stoll(raw_val);
             if (v > std::numeric_limits<int>::max() || v < std::numeric_limits<int>::min()) {
               is_ovf = true;
             } else {
               count_nums.push_back(static_cast<int>(v));
             }
          }
        } catch (...) {
          is_ovf = true;
        }
        while (std::cin.peek() == ' ') std::cin.ignore();
      }
      
      list_for_sol.push_back(std::make_pair(s, std::move(count_nums)));
      if (std::cin.peek() == '\n') std::cin.ignore();
    }

    if (list_for_sol.IsEmpty()) {
      if (is_ovf) { std::cerr << "overflow\n"; return 1; }
      std::cout << "0\n";
      return 0;
    }

    petrov::LIter<std::pair<std::string, petrov::List<int>>> q_names = list_for_sol.begin();
    while (q_names != list_for_sol.end()) {
      std::cout << q_names->first;
      ++q_names;
      if (q_names != list_for_sol.end()) std::cout << ' ';
    }
    std::cout << '\n';

    if (is_ovf) {
      std::cerr << "overflow\n";
      return 1;
    }

    bool all_empty = true;
    for (auto it = list_for_sol.begin(); it != list_for_sol.end(); ++it) {
      if (!it->second.IsEmpty()) { all_empty = false; break; }
    }
    if (all_empty) { std::cout << "0\n"; return 0; }

    petrov::List<petrov::LIter<int>> iters;
    for (auto it = list_for_sol.begin(); it != list_for_sol.end(); ++it) {
      iters.push_back(it->second.begin());
    }

    petrov::List<int> sums;
    while (true) {
      int current_row_sum = 0;
      bool isleft = false;
      petrov::List<int> curRows;
      auto its_it = iters.begin();
      auto data_it = list_for_sol.begin();

      while (its_it != iters.end()) {
        if (*its_it != data_it->second.end()) {
          int val = **its_it;
          curRows.push_back(val);
          current_row_sum = sum(current_row_sum, val);
          ++(*its_it);
          isleft = true;
        }
        ++its_it; ++data_it;
      }
      if (!isleft) break;

      for (auto e = curRows.begin(); e != curRows.end(); ) {
        std::cout << *e;
        if (++e != curRows.end()) std::cout << ' ';
      }
      std::cout << '\n';
      sums.push_back(current_row_sum);
    }

    for (auto s_it = sums.begin(); s_it != sums.end(); ) {
      std::cout << *s_it;
      if (++s_it != sums.end()) std::cout << ' ';
    }
    std::cout << '\n';

  } catch (...) {
    std::cerr << "overflow\n";
    return 1;
  }
  return 0;
}

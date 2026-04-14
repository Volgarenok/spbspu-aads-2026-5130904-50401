#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include <stdexcept>
#include <sstream>
#include "list.hpp"

int safe_sum(int a, int b)
{
  long long res = static_cast<long long>(a) + b;
  if (res > std::numeric_limits<int>::max() || res < std::numeric_limits<int>::min())
  {
    throw std::overflow_error("overflow");
  }
  return static_cast<int>(res);
}

int main()
{
  petrov::List<std::pair<std::string, petrov::List<long long>>> list_for_sol;
  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty()) continue;

    std::stringstream ss(line);
    std::string name;
    if (!(ss >> name)) continue;

    petrov::List<long long> count_nums;
    std::string val_str;
    while (ss >> val_str)
    {
      try
      {
        unsigned long long val = std::stoull(val_str);
        count_nums.push_back(static_cast<long long>(val));
      }
      catch (...)
      {
        std::cerr << "overflow" << std::endl;
        return 1;
      }
    }
    list_for_sol.push_back(std::make_pair(name, std::move(count_nums)));
  }

  if (list_for_sol.IsEmpty())
  {
    return 0;
  }

  petrov::LIter<std::pair<std::string, petrov::List<long long>>> q_names = list_for_sol.begin();
  while (q_names != list_for_sol.end())
  {
    std::cout << q_names->first;
    ++q_names;
    if (q_names != list_for_sol.end()) std::cout << ' ';
  }
  std::cout << '\n';

  petrov::List<petrov::LIter<long long>> iters;
  petrov::LIter<std::pair<std::string, petrov::List<long long>>> q_init = list_for_sol.begin();
  while (q_init != list_for_sol.end())
  {
    iters.push_back(q_init->second.begin());
    ++q_init;
  }

  petrov::List<int> result_sums;
  while (1)
  {
    int current_row_sum = 0;
    bool has_data = 0;
    petrov::List<long long> current_row_vals;

    petrov::LIter<petrov::LIter<long long>> its_it = iters.begin();
    petrov::LIter<std::pair<std::string, petrov::List<long long>>> data_it = list_for_sol.begin();

    while (its_it != iters.end())
    {
      if (*its_it != data_it->second.end())
      {
        long long val = **its_it;
        if (val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min())
        {
          std::cerr << "overflow" << std::endl;
          return 1;
        }

        current_row_vals.push_back(val);
        current_row_sum = safe_sum(current_row_sum, static_cast<int>(val));
        ++(*its_it);
        has_data = 1;
      }
      ++its_it;
      ++data_it;
    }

    if (!has_data) break;

    petrov::LIter<long long> e_it = current_row_vals.begin();
    while (e_it != current_row_vals.end())
    {
      std::cout << *e_it;
      ++e_it;
      if (e_it != current_row_vals.end()) std::cout << ' ';
    }
    std::cout << '\n';
    result_sums.push_back(current_row_sum);
  }

  if (!result_sums.IsEmpty())
  {
    petrov::LIter<int> s_it = result_sums.begin();
    while (s_it != result_sums.end())
    {
      std::cout << *s_it;
      ++s_it;
      if (s_it != result_sums.end()) std::cout << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}

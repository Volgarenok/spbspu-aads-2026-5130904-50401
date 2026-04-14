#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include "list.hpp"

int main()
{
  petrov::List<std::pair<std::string, petrov::List<unsigned long long>>> list_for_sol;
  std::string name;

  while (std::cin >> name)
  {
    petrov::List<unsigned long long> count_nums;
    unsigned long long t = 0;

    while (std::cin >> t)
    {
      count_nums.push_back(t);
    }

    list_for_sol.push_back(std::make_pair(name, std::move(count_nums)));

    if (std::cin.bad())
    {
      return 1;
    }

    std::cin.clear();
  }

  if (list_for_sol.IsEmpty())
  {
    std::cout << "0\n";
    return 0;
  }

  petrov::LIter<std::pair<std::string, petrov::List<unsigned long long>>> q_names = list_for_sol.begin();
  while (q_names != list_for_sol.end())
  {
    std::cout << q_names->first;
    ++q_names;
    if (q_names != list_for_sol.end())
    {
      std::cout << ' ';
    }
  }
  std::cout << '\n';

  bool all_empty = 1;
  petrov::LIter<std::pair<std::string, petrov::List<unsigned long long>>> check_empty = list_for_sol.begin();
  while (check_empty != list_for_sol.end())
  {
    if (!check_empty->second.IsEmpty())
    {
      all_empty = 0;
      break;
    }
    ++check_empty;
  }

  if (all_empty)
  {
    std::cout << "0\n";
    return 0;
  }

  petrov::List<petrov::LIter<unsigned long long>> iters;
  petrov::LIter<std::pair<std::string, petrov::List<unsigned long long>>> q_init = list_for_sol.begin();
  while (q_init != list_for_sol.end())
  {
    iters.push_back(q_init->second.begin());
    ++q_init;
  }

  petrov::List<unsigned long long> sums;
  while (1)
  {
    unsigned long long current_row_sum = 0;
    bool isleft = 0;
    bool first_in_row = 1;
    petrov::LIter<petrov::LIter<unsigned long long>> its_it = iters.begin();
    petrov::LIter<std::pair<std::string, petrov::List<unsigned long long>>> data_it = list_for_sol.begin();

    while (its_it != iters.end())
    {
      if (*its_it != data_it->second.end())
      {
        unsigned long long val = **its_it;
        isleft = 1;

        if (!first_in_row)
        {
          std::cout << ' ';
        }
        std::cout << val;
        first_in_row = 0;

        if (std::numeric_limits<unsigned long long>::max() - current_row_sum < val)
        {
          std::cout << '\n';
          std::cerr << "overflow\n";
          return 1;
        }
        current_row_sum += val;
        ++(*its_it);
      }
      ++its_it;
      ++data_it;
    }

    if (!isleft)
    {
      break;
    }

    std::cout << '\n';
    sums.push_back(current_row_sum);
  }

  petrov::LIter<unsigned long long> s_it = sums.begin();
  while (s_it != sums.end())
  {
    std::cout << *s_it;
    ++s_it;
    if (s_it != sums.end())
    {
      std::cout << ' ';
    }
  }
  std::cout << '\n';

  return 0;
}

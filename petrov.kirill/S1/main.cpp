#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include "list.hpp"

int main()
{
  petrov::List<std::pair<std::string, petrov::List< size_t >>> list_for_sol;
  std::string name;

  while (std::cin >> name)
  {
    petrov::List< size_t > count_nums;
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

  if (list_for_sol.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  petrov::LIter<std::pair<std::string, petrov::List< size_t >>> q_names = list_for_sol.begin();
  while (q_names != list_for_sol.end())
  {
    std::cout << q_names->first;
    ++q_names;
    while (q_names != list_for_sol.end())
    {
      std::cout << ' ' << q_names->first;
      ++q_names;
    }
  }
  std::cout << '\n';

  bool all_empty = 1;
  petrov::LIter<std::pair<std::string, petrov::List< size_t >>> check_empty = list_for_sol.begin();
  while (check_empty != list_for_sol.end())
  {
    if (!check_empty->second.empty())
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

  petrov::List<petrov::LIter< size_t >> iters;
  petrov::LIter<std::pair<std::string, petrov::List< size_t >>> q_init = list_for_sol.begin();
  while (q_init != list_for_sol.end())
  {
    iters.push_back(q_init->second.begin());
    ++q_init;
  }

  petrov::List< size_t > sums;
  bool isleft = 1;
  while (isleft)
  {
    unsigned long long current_row_sum = 0;
    bool first_in_row = 1;
    isleft = 0;
    petrov::LIter<petrov::LIter< size_t >> its_it = iters.begin();
    petrov::LIter<std::pair<std::string, petrov::List< size_t >>> data_it = list_for_sol.begin();

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

        if (std::numeric_limits< size_t >::max() - current_row_sum < val)
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

  petrov::LIter< size_t > s_it = sums.begin();
  while (s_it != sums.end())
  {
    std::cout << *s_it;
    ++s_it;
    while (s_it != sums.end())
    {
      std::cout << ' ' << *s_it;
      ++s_it;
    }
  }
  std::cout << '\n';

  return 0;
}

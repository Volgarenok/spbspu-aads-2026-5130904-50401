#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include <stdexcept>
#include "list.hpp"

int sum(int a, int b)
{
  if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
      (b < 0 && a < std::numeric_limits<int>::min() - b))
  {
    throw std::overflow_error("overflow");
  }
  return a + b;
}

int main()
{
  petrov::List<std::pair<std::string, petrov::List<int>>> list_for_sol;
  std::string s;
  bool is_ovf = false;

  try
  {
    while (std::cin >> s)
    {
      petrov::List<int> count_nums;
      while (std::cin.peek() == ' ')
      {
        std::cin.ignore();
      }

      while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
      {
        while (std::cin.peek() == ' ')
        {
          std::cin.ignore();
        }
        if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
        {
          break;
        }

        std::string raw_val;
        if (!(std::cin >> raw_val))
        {
          break;
        }

        try
        {
          // Простейшая проверка длины, чтобы stoll не выкинул out_of_range
          if (raw_val.length() > 19 || (raw_val.length() > 0 && raw_val[0] != '-' && raw_val.length() >= 11))
          {
            is_ovf = true;
          }
          else
          {
            long long v = std::stoll(raw_val);
            if (v > std::numeric_limits<int>::max() || v < std::numeric_limits<int>::min())
            {
              is_ovf = true;
            }
            else
            {
              count_nums.push_back(static_cast<int>(v));
            }
          }
        }
        catch (...)
        {
          is_ovf = true;
        }
      }
      list_for_sol.push_back(std::make_pair(s, std::move(count_nums)));
      if (std::cin.peek() == '\n')
      {
        std::cin.ignore();
      }
    }

    if (list_for_sol.IsEmpty())
    {
      if (is_ovf)
      {
        std::cerr << "overflow\n";
        return 1;
      }
      std::cout << "0\n";
      return 0;
    }

    // ВЫВОД ИМЕН (Stdout)
    petrov::LIter<std::pair<std::string, petrov::List<int>>> q_names = list_for_sol.begin();
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

    // ЗДЕСЬ КРИЧИМ ПРО ОШИБКУ ЧТЕНИЯ (Stderr)
    if (is_ovf)
    {
      std::cerr << "overflow\n";
      return 1;
    }

    bool all_empty = true;
    petrov::LIter<std::pair<std::string, petrov::List<int>>> check_empty = list_for_sol.begin();
    while (check_empty != list_for_sol.end())
    {
      if (!check_empty->second.IsEmpty())
      {
        all_empty = false;
        break;
      }
      ++check_empty;
    }

    if (all_empty)
    {
      std::cout << "0\n";
      return 0;
    }

    petrov::List<petrov::LIter<int>> iters;
    petrov::LIter<std::pair<std::string, petrov::List<int>>> q_init = list_for_sol.begin();
    while (q_init != list_for_sol.end())
    {
      iters.push_back(q_init->second.begin());
      ++q_init;
    }

    petrov::List<int> sums;
    while (1)
    {
      int current_row_sum = 0;
      bool isleft = false;
      petrov::List<int> curRows;
      petrov::LIter<petrov::LIter<int>> its_it = iters.begin();
      petrov::LIter<std::pair<std::string, petrov::List<int>>> data_it = list_for_sol.begin();

      while (its_it != iters.end())
      {
        if (*its_it != data_it->second.end())
        {
          int val = **its_it;
          curRows.push_back(val);
          current_row_sum = sum(current_row_sum, val);
          ++(*its_it);
          isleft = true;
        }
        ++its_it;
        ++data_it;
      }

      if (!isleft)
      {
        break;
      }

      petrov::LIter<int> e_it = curRows.begin();
      while (e_it != curRows.end())
      {
        std::cout << *e_it;
        ++e_it;
        if (e_it != curRows.end())
        {
          std::cout << ' ';
        }
      }
      std::cout << '\n';
      sums.push_back(current_row_sum);
    }

    petrov::LIter<int> s_it = sums.begin();
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
  }
  catch (...)
  {
    std::cerr << "overflow\n";
    return 1;
  }

  return 0;
}

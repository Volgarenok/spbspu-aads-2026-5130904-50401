#include <iostream>
#include <limits>
#include "bidir_list.hpp"

int main()
{
  size_t max_num = std::numeric_limits< size_t >::max();
  using stringListPair = std::pair< std::string, tarasenko::BidirList< size_t > >;
  auto list = tarasenko::BidirList< stringListPair >();
  std::string current_str;

  while (std::cin >> current_str)
  {
    auto current_list = tarasenko::BidirList< size_t >();
    size_t current_val = 0;
    while (std::cin >> current_val)
    {
      current_list.push_back(current_val);
    }
    list.push_back(std::make_pair(current_str, current_list));
    std::cin.clear();
  }

  if (list.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  auto it = list.begin();
  std::cout << it->first;
  ++it;
  for (; it != list.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  size_t max_size = 0;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    max_size = std::max(max_size, it->second.size());
  }
  if (max_size == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  using IterSizePair = std::pair< tarasenko::ListIter< size_t >, size_t >;
  auto iter_list = tarasenko::BidirList< IterSizePair >();
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    iter_list.push_back(std::make_pair(it->second.begin(), it->second.size()));
  }

  auto sums_list = tarasenko::BidirList< size_t >();
  for (size_t i = 0; i < max_size; ++i)
  {
    sums_list.push_back(0);
  }

  auto sums_it = sums_list.begin();
  for (size_t i = 0; i < max_size; ++i)
  {
    bool flag = true;
    for (auto it = iter_list.begin(); it != iter_list.end(); ++it)
    {
      if ((*it).second > i)
      {
        size_t current_num = *((*it).first);
        if (flag)
        {
          std::cout << current_num;
          flag = false;
        }
        else
        {
          std::cout << " " << current_num;
        }
        if (max_num - current_num >= *sums_it)
        {
          *sums_it += current_num;
        }
        else
        {
          std::cout << "\n";
          std::cerr << "addition overflow\n";
          return 1;
        }
        ++((*it).first);
      }
    }
    ++sums_it;
    std::cout << "\n";
  }

  sums_it = sums_list.begin();
  std::cout << *sums_it;
  ++sums_it;
  for (; sums_it != sums_list.end(); ++sums_it)
  {
    std::cout << " " << *sums_it;
  }
  std::cout << "\n";
}

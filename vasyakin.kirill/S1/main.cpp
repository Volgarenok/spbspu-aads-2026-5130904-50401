#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstddef>
#include "list.hpp"

int main()
{
  vasyakin::List< std::pair< std::string, vasyakin::List< size_t > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    vasyakin::List< size_t > numbers;
    size_t num = 0;
    vasyakin::detail::Node< size_t >* last_num = nullptr;

    while (std::cin >> num)
    {
      if (last_num == nullptr)
      {
        last_num = numbers.insert(numbers.getFake(), num);
      }
      else
      {
        last_num = numbers.insert(last_num, num);
      }
    }
    std::cin.clear();

    sequences.pushBack(std::make_pair(name, std::move(numbers)));
  }

  if (sequences.begin() == sequences.end())
  {
    std::cout << "0\n";
    return 0;
  }

  auto seq_it = sequences.begin();
  std::cout << seq_it->first;
  ++seq_it;
  while (seq_it != sequences.end())
  {
    std::cout << " " << seq_it->first;
    ++seq_it;
  }
  std::cout << '\n';

  size_t max_len = 0;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t len = 0;
    for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
    {
      ++len;
    }
    if (len > max_len)
    {
      max_len = len;
    }
  }

  vasyakin::List< vasyakin::List< size_t > > transposed;
  vasyakin::detail::Node< vasyakin::List< size_t > >* last_trans = nullptr;
  bool hasNumbers = false;

  for (size_t pos = 0; pos < max_len; ++pos)
  {
    vasyakin::List< size_t > new_seq;

    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      auto nit = sit->second.cbegin();
      size_t curr = 0;
      while (curr < pos && nit != sit->second.cend())
      {
        ++curr;
        ++nit;
      }

      if (nit != sit->second.cend())
      {
        new_seq.pushBack(*nit);
        hasNumbers = true;
      }
    }

    if (new_seq.begin() != new_seq.end())
    {
      if (last_trans == nullptr)
      {
        last_trans = transposed.insert(transposed.getFake(), std::move(new_seq));
      }
      else
      {
        last_trans = transposed.insert(last_trans, std::move(new_seq));
      }
    }
  }

  if (hasNumbers)
  {
    for (auto tit = transposed.begin(); tit != transposed.end(); ++tit)
    {
      for (auto nit = tit->cbegin(); nit != tit->cend(); ++nit)
      {
        if (nit != tit->cbegin())
        {
          std::cout << " ";
        }
        std::cout << *nit;
      }
      std::cout << '\n';
    }

    vasyakin::List< size_t > sums;
    for (auto tit = transposed.cbegin(); tit != transposed.cend(); ++tit)
    {
      size_t sum = 0;
      try
      {
        for (auto nit = tit->cbegin(); nit != tit->cend(); ++nit)
        {
          if (sum > vasyakin::max - *nit)
          {
            throw std::overflow_error("Sum overflow");
          }
          sum += *nit;
        }
      }
      catch (const std::overflow_error& e)
      {
        std::cerr << e.what() << "\n";
        return 1;
      }
      sums.pushBack(sum);
    }

    for (auto sit = sums.begin(); sit != sums.end(); ++sit)
    {
      if (sit != sums.begin())
      {
        std::cout << " ";
      }
      std::cout << *sit;
    }
    std::cout << '\n';
  }
  else
  {
    std::cout << "0\n";
  }

  return 0;
}

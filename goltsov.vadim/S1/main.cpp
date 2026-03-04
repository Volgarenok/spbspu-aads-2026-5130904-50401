#include <iostream>
#include "mylist.hpp"
#include <string>

goltsov::List< std::pair< std::string, goltsov::List< size_t > > > getData(std::istream& in, size_t& size)
{
  goltsov::List< std::pair< std::string, goltsov::List< size_t > > > result;
  goltsov::LIter< std::pair< std::string, goltsov::List< size_t > > > i(nullptr);
  std::string name;
  while (in >> name)
  {
    size += 1;
    goltsov::List< size_t > numbers;
    size_t number;
    goltsov::LIter< size_t > j(nullptr);
    while (in >> number)
    {
      numbers.insert(j, number);
      j = j.next();
      char next = in.peek();
      if (next == '\n' || next == EOF)
      {
        break;
      }
    }
    if (!in.eof())
    {
      in.clear();
    }
    result.insert(i, {name, numbers});
    i = i.next();
  }
  return result;
}

void push_back(size_t** sums, size_t& n, size_t a)
{
  size_t* new_sums = new size_t[n + 1];
  for (size_t i = 0; i < n; ++i)
  {
    new_sums[i] = sums[0][i];
  }
  new_sums[n] = a;
  delete[] (* sums);
  sums[0] = new_sums;
  n += 1;
}

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< size_t > > > data = getData(std::cin, size);
  goltsov::LIter< std::pair< std::string, goltsov::List< size_t > > > it = data.begin();
  size_t* sums = nullptr;
  size_t n = 0;
  goltsov::LIter< size_t >* its = new goltsov::LIter< size_t >[size];
  for (size_t i = 0; i < size; ++i)
  {
    if (i != size - 1)
    {
      std::cout << (* it).first << ' ';
    }
    else
    {
      std::cout << (* it).first << '\n';
    }
    try
    {
      its[i] = (* it).second.begin();
    }
    catch(...)
    {
      delete[] its;
      delete[] sums;
    }
    it = it.next();
  }
  it = data.begin();
  bool all = 0;
  size_t real_ind = 0;
  while (!all)
  {
    all = 1;
    size_t sum = 0;
    for (size_t i = 0; i < size; ++i)
    {
      if (its[i].hasNext())
      {
        if (all == 1)
        {
          std::cout << (* its[i]);
          sum += (* its[i]);
        }
        else
        {
          std::cout << ' ' << (* its[i]);
          sum += (* its[i]);
        }
        all = 0;
        its[i] = its[i].next();
      }
    }
    if (!all)
    {
      try
      {
        push_back(& sums, n, sum);
      }
      catch(...)
      {
        delete[] sums;
        delete[] its;
      }
      std::cout << '\n';
    }
  }
  for (size_t i = 0; i < n; ++i)
  {
    if (i != n - 1)
    {
      std::cout << sums[i] << ' ';
    }
    else
    {
      std::cout << sums[i] << '\n';
    }
  }
}

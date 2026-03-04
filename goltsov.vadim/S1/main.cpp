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
    result.insert(i, {name, numbers});
    i = i.next();
  }
  return result;
}

int main()
{
  size_t size = 0;
  goltsov::List< std::pair< std::string, goltsov::List< size_t > > > data = getData(std::cin, size);
  goltsov::LIter< std::pair< std::string, goltsov::List< size_t > > > it = data.begin();
  size_t* sums = new size_t[size];
  goltsov::LIter< size_t >* its = new goltsov::LIter< size_t >[size];
  for (size_t i = 0; i < size; ++i)
  {
    if (i != size - 1)
    {
      std::cout << (*it).first << ' ';
    }
    else
    {
      std::cout << (*it).first << '\n';
    }
    sums[i] = 0;
    its[i] = (*it).second.begin();
    it = it.next();
  }
  it = data.begin();
  bool all = 0;
  size_t real_ind = 0;
  while (!all)
  {
    all = 1;
    real_ind = 0;
    for (size_t i = 0; i < size; ++i)
    {
      if (its[i].hasNext())
      {
        if (all == 1)
        {
          std::cout << (* its[i]);
        }
        else
        {
          std::cout << ' ' << (* its[i]);
        }
        all = 0;
        sums[real_ind] += (* its[i]);
        real_ind++;
        its[i] = its[i].next();
      }
    }
    std::cout << '\n';
  }
  for (size_t i = 0; i < size; ++i)
  {
    if (i != size - 1)
    {
      std::cout << sums[i] << ' ';
    }
    else
    {
      std::cout << sums[i] << '\n';
    }
  }
}

#include "data_processing.hpp"
#include <iostream>
#include <limits>

size_t goltsov::sumWithCheck(size_t a, size_t b)
{
  size_t max_size_t = std::numeric_limits< size_t >::max();
  if (a <= max_size_t - b)
  {
    return a + b;
  }
  else
  {
    throw std::overflow_error("The sum is too big");
  }
}

void goltsov::getData(std::istream& in, List< std::pair< std::string, List< size_t > > >& result, size_t& size)
{
  LIter< std::pair< std::string, List< size_t > > > i = result.beforeBegin();
  std::string name;
  while (in >> name && !in.eof())
  {
    size += 1;
    List< size_t > numbers;
    size_t number = 0;
    LIter< size_t > j = numbers.beforeBegin();
    while (in >> number && !in.eof())
    {
      j = numbers.insertAfter(j, number);
    }
    if (in.fail())
    {
      in.clear();
    }
    i = result.insertAfter(i, std::pair< std::string, List< size_t > >{name, numbers});
  }
}

std::ostream& goltsov::printResult(std::ostream& out, List< std::pair< std::string, List< size_t > > >& data, size_t size)
{
  if (size == 0)
  {
    out << 0;
    return out;
  }
  List< LIter< size_t > > list_its;
  printResultNamesAndMakeListOfIterators(out, data, list_its);
  out << '\n';
  printResultNumbersAndSums(out, list_its);
  return out;
}

std::ostream& goltsov::printResultNamesAndMakeListOfIterators(std::ostream& out, goltsov::List< std::pair< std::string,
  goltsov::List< size_t > > >& data, List< LIter< size_t > >& list_its)
{
  LIter< std::pair< std::string, List< size_t > > > it = data.begin();
  LIter< LIter< size_t > > it_list_its = list_its.beforeBegin();
  out << (*it).first;
  it_list_its = list_its.insertAfter(it_list_its, (*it).second.begin());
  ++it;
  for (; it != data.end(); ++it)
  {
    out << ' ' << (*it).first;
    it_list_its = list_its.insertAfter(it_list_its, (*it).second.begin());
  }
  return out;
}

std::ostream& goltsov::printResultNumbersAndSums(std::ostream& out, List< LIter< size_t > >& list_its)
{
  bool all = 0;
  bool without_overflows = 1;
  List< size_t > sums;
  size_t size_sums = 0;
  LIter< size_t > it_sums = sums.beforeBegin();
  while (!all)
  {
    all = 1;
    size_t sum = 0;
    for (LIter< LIter< size_t > > list_it = list_its.begin(); list_it != list_its.end(); ++list_it)
    {
      bool has_next;
      try
      {
        LIter< size_t > temp = (*list_it);
        ++temp;
        has_next = true;
      }
      catch (...)
      {
        has_next = false;
      }
      if (has_next)
      {
        if (all == 1)
        {
          out << (*(*list_it));
          try
          {
            sum = sumWithCheck(sum, (*(*list_it)));
          }
          catch (...)
          {
            without_overflows = 0;
          }
        }
        else
        {
          out << ' ' << (*(*list_it));
          try
          {
            sum = sumWithCheck(sum, (*(*list_it)));
          }
          catch (...)
          {
            without_overflows = 0;
          }
        }
        all = 0;
        ++(*list_it);
      }
    }
    if (!all)
    {
      it_sums = sums.insertAfter(it_sums, sum);
      size_sums += 1;
      out << '\n';
    }
  }
  if (!without_overflows)
  {
    throw std::overflow_error("The sum is too big");
  }
  if (size_sums != 0)
  {
    out << (*sums.begin());
    LIter< size_t > it = sums.begin();
    ++it;
    for (; it != sums.end(); ++it)
    {
      out << ' ' << (*it);
    }
  }
  else
  {
    std::cout << 0;
  }
  return out;
}

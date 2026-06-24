#include "functions.hpp"
#include <istream>
#include <iostream>
#include <algorithm>
#include <limits>

void petrov::mkData(std::istream& input, List< std::pair< std::string, List< size_t > > >& data)
{
  std::string name;
  while(input >> name)
  {
    List< size_t > nums;
    size_t num;
    LIter< size_t > inPos = nums.back();
    while (input >> num)
    {
      inPos = nums.insert_after(inPos, num);
    }
    data.insert_after(data.back(), {name, nums});

    if(input.eof())
    {
      break;
    }
    else
    {
      input.clear();
    }
  }
}

size_t petrov::maxLen(const List< std::pair< std::string, List< size_t > > >& data)
{
  size_t max = 0;
  LCIter< std::pair< std::string, List< size_t > > > it = data.begin();
  while (it != data.end())
  {
    max = std::max(max, it->second.size());
    ++it;
  }
  return max;
}

bool petrov::canShowItem(const List< size_t >& nums, size_t istr)
{
  return nums.size() >= istr;
}

void petrov::printAllList(List< std::pair< std::string, List< size_t > > >& data, std::ostream& out)
{
  if(data.size() < 1)
  {
    out << "0";
    return;
  }

  LIter< std::pair< std::string, List< size_t > > > it = data.begin();
  out << it->first;
  ++it;

  for (size_t i = 1; i < data.size(); ++i, ++it)
  {
    out << " " << it->first;
  }
  out << "\n";

  size_t maxListLen = 0;
  it = data.begin();
  for (size_t i = 0; i < data.size(); ++i, ++it)
  {
    maxListLen = std::max(maxListLen, it->second.size());
  }

  if (maxListLen == 0)
  {
    out << "0";
    return;
  }

  List< size_t > sums;
  for (size_t i = 0; i < maxListLen; ++i)
  {
    sums.push_front(0);
  }

  const size_t maxValue = std::numeric_limits< size_t >::max();

  LIter< size_t > sumIt = sums.begin();
  for (size_t row = 0; row < maxListLen; ++row, ++sumIt)
  {
    it = data.begin();
    bool found = false;
    size_t pos = 0;

    for (; pos < data.size(); ++pos, ++it)
    {
      List< size_t >& seq = it->second;

      if (row < seq.size())
      {
        LIter< size_t > sit = seq.begin();
        for (size_t j = 0; j < row; ++j)
        {
          ++sit;
        }

        out << *sit;

        if (maxValue - *sit < *sumIt)
        {
          out << "\n";
          throw std::overflow_error("Overflow");
        }

        *sumIt += *sit;
        found = true;

        ++it;
        ++pos;
        break;
      }
    }

    if (found)
    {
      for (; pos < data.size(); ++pos, ++it)
      {
        List< size_t >& seq = it->second;

        if (row < seq.size())
        {
          LIter< size_t > sit = seq.begin();
          for (size_t j = 0; j < row; ++j)
          {
            ++sit;
          }

          out << " " << *sit;

          if (maxValue - *sit < *sumIt)
          {
            out << "\n";
            throw std::overflow_error("Overflow");
          }

          *sumIt += *sit;
        }
      }
    }
    out << '\n';
  }

  sumIt = sums.begin();
  out << *sumIt;
  ++sumIt;

  for (size_t i = 1; i < sums.size(); ++i, ++sumIt)
  {
    out << " " << *sumIt;
  }
}

void petrov::showSums(List< size_t >& sums, std::ostream& out)
{
  if (sums.begin() != sums.end())
  {
    LIter< size_t > it = sums.begin();
    out << *it;
    ++it;
    while (it != sums.end())
    {
      out << " " << *it;
      ++it;
    }
    out << "\n";
  }
  else
  {
    out << "0\n";
  }
}

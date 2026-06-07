#include <iostream>
#include <iomanip>
#include "list.hpp"
#include "functions.hpp"

void donkeev::read(List< std::pair< std::string, List< size_t > > >& data, std::istream& input)
{
  std::string seqName;
  while (input >> seqName)
  {
    size_t value;
    donkeev::List< size_t > inner{};
    while (input >> value)
    {
      inner.pushBack(value);
    }
    if (input.eof())
    {
      data.pushBack({seqName, inner});
      break;
    }
    if (input.fail())
    {
      input.clear();
      data.pushBack({seqName, inner});
      continue;
    }
  }
}
bool donkeev::areAnyvalues(const List< std::pair< std::string, List< size_t > > >& data)
{
  LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
  for (size_t i = 0; i < data.size(); ++i)
  {
    if (!outIt->second.empty())
    {
      return true;
    }
    ++outIt;
  }
  return false;
}
size_t donkeev::getMaxSize(const List< std::pair< std::string, List< size_t > > >& data)
{
  size_t maximum = 0;
  LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
  for (size_t i = 0; i < data.size(); ++i)
  {
    List< size_t > innerList = outIt->second;
    maximum = std::max(maximum, innerList.size());
    ++outIt;
  }
  return maximum;
}
bool donkeev::checkOnOverflow(const List< std::pair< std::string, List< size_t > > >& data)
{
  size_t maximumIteration = getMaxSize(data);
  size_t iterationCount = 0;
  while (iterationCount < maximumIteration)
  {
    size_t overflow = 0;
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
    for (size_t i = 0; i < data.size(); ++i)
    {
      LCIter< size_t > innerIt = outIt->second.cbegin();
      if (outIt->second.size() > iterationCount)
      {
        innerIt += iterationCount;
        if ((overflow + *innerIt) < overflow)
        {
          return true;
        }
        overflow += *innerIt;
      }
      ++outIt;
    }
    ++iterationCount;
  }
  return false;
}

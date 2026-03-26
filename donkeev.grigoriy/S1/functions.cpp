#include <iostream>
#include <iomanip>
#include "list.hpp"

namespace donkeev
{
  void read(List< std::pair< std::string, List< size_t > > >& data, std::istream& input)
  {

    std::string seqName;
    while (input >> seqName)
    {
      size_t value;
      List< size_t > inner{};
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
  bool areAnyvalues(const List< std::pair< std::string, List< size_t > > >& data)
  {
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.begin();
    for (size_t i = 0; i < data.size(); ++i)
    {
      if (!outIt->second.isEmpty())
      {
        return true;
      }
      ++outIt;
    }
    return false;
  }
  size_t getMaxSize(const List< std::pair< std::string, List< size_t > > >& data)
  {
    size_t maximum = 0;
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.begin();
    for (size_t i = 0; i < data.size(); ++i)
    {
      List< size_t > innerList = outIt->second;
      maximum = std::max(maximum, innerList.size());
    }
    return maximum;
  }
  void printInfo(const List< std::pair< std::string, List< size_t > > >& data, std::ostream& output)
  {
    if (data.isEmpty())
    {
      output << "0" << "\n";
      return;
    }
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.begin();
    output << outIt->first;
    outIt++;
    for (size_t i = 1; i < data.size(); ++i)
    {
      output << " " << outIt->first;
      outIt++;
    }
    output << "\n";
    size_t iterationCount = 0;
    size_t maximumIteration = getMaxSize(data);
    while (iterationCount < maximumIteration)
    {
      outIt = data.begin();
      for (size_t i = 0; i < data.size(); ++i)
      {
        LCIter< size_t > innerIt = outIt->second.begin();
        if (outIt->second.size() > iterationCount)
        {
          innerIt += iterationCount;
          output << *innerIt << " ";
        }
        ++outIt;
      }
      output << "\n";
      ++iterationCount;
    }
    if (!areAnyvalues(data))
    {
      output << 0 << "\n";
    }
  }
}

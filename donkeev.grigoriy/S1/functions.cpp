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
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
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
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
    for (size_t i = 0; i < data.size(); ++i)
    {
      List< size_t > innerList = outIt->second;
      maximum = std::max(maximum, innerList.size());
      ++outIt;
    }
    return maximum;
  }
  bool checkOnOverflow(const List< std::pair< std::string, List< size_t > > >& data)
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
  void printInfo(const List< std::pair< std::string, List< size_t > > >& data, std::ostream& output)
  {
    if (data.isEmpty())
    {
      output << "0" << "\n";
      return;
    }
    size_t maximumIteration = getMaxSize(data);
    size_t* sumArray = nullptr;
    if (maximumIteration)
    {
      sumArray = new size_t[maximumIteration]{0};
    }
    size_t sumIteration = 0;
    LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
    output << outIt->first;
    outIt++;
    for (size_t i = 1; i < data.size(); ++i)
    {
      output << " " << outIt->first;
      outIt++;
    }
    output << "\n";
    if (checkOnOverflow(data))
    {
      size_t iterationCount = 0;
      while (iterationCount < maximumIteration)
      {
        outIt = data.cbegin();
        bool firstPrint = true;
        for (size_t i = 0; i < data.size(); ++i)
        {
          LCIter< size_t > innerIt = outIt->second.cbegin();
          if (outIt->second.size() > iterationCount)
          {
            if (firstPrint)
            {
              innerIt += iterationCount;
              output << *innerIt;
              firstPrint = false;
            }
            else
            {
              innerIt += iterationCount;
              output << " " << *innerIt;
            }
          }
          ++outIt;
        }
        output << "\n";
        ++iterationCount;
      }
      throw std::overflow_error("Sum of elements is overflow");
    }
    size_t iterationCount = 0;
    while (iterationCount < maximumIteration)
    {
      outIt = data.cbegin();
      bool firstPrint = true;
      for (size_t i = 0; i < data.size(); ++i)
      {
        LCIter< size_t > innerIt = outIt->second.cbegin();
        if (outIt->second.size() > iterationCount)
        {
          if (firstPrint)
          {
            innerIt += iterationCount;
            sumArray[sumIteration] += *innerIt;
            output << *innerIt;
            firstPrint = false;
          }
          else
          {
            innerIt += iterationCount;
            sumArray[sumIteration] += *innerIt;
            output << " " << *innerIt;
          }
        }
        ++outIt;
      }
      ++sumIteration;
      output << "\n";
      ++iterationCount;
    }
    if (!areAnyvalues(data))
    {
      output << 0 << "\n";
    }
    size_t i = 0;
    if (maximumIteration)
    {
      output << sumArray[i];
    }
    ++i;
    for (; i < maximumIteration; ++i)
    {
      output << " " << sumArray[i];
    }
    if (maximumIteration)
    {
      output << "\n";
    }
    delete [] sumArray;
  }
}

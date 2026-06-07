#include <iostream>
#include "list.hpp"
#include "functions.hpp"

int main()
{
  using namespace donkeev;
  List< std::pair< std::string, List< size_t > > > data{};
  read(data, std::cin);
  if (data.empty())
  {
    std::cout << "0" << '\n';
    return 0;
  }
  size_t maximumIteration = getMaxSize(data);
  size_t* sumArray = nullptr;
  if (maximumIteration)
  {
    sumArray = new size_t[maximumIteration]{0};
  }
  size_t sumIteration = 0;
  LCIter< std::pair< std::string, List< size_t > > > outIt = data.cbegin();
  std::cout << outIt->first;
  outIt++;
  for (size_t i = 1; i < data.size(); ++i)
  {
    std::cout << " " << outIt->first;
    outIt++;
  }
  std::cout << "\n";
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
            std::cout << *innerIt;
            firstPrint = false;
          }
          else
          {
            innerIt += iterationCount;
            std::cout << " " << *innerIt;
          }
        }
        ++outIt;
      }
      std::cout << "\n";
      ++iterationCount;
    }

    delete [] sumArray;

    std::cerr << "Sum of elements is overflow\n";
    return 1;
  }
  size_t iterationCount = 0;
  while (iterationCount < maximumIteration)
  {
    outIt = data.cbegin();
    LCIter< size_t > innerIt = outIt->second.cbegin();
    if (outIt->second.size() > iterationCount)
    {
      innerIt += iterationCount;
      sumArray[sumIteration] += *innerIt;
      std::cout << *innerIt;
    }
    ++outIt;

    for (size_t i = 1; i < data.size(); ++i)
    {
      if (outIt->second.size() > iterationCount)
      {
        innerIt += iterationCount;
        sumArray[sumIteration] += *innerIt;
        std::cout << " " << *innerIt;
      }

      ++outIt;
    }
    ++sumIteration;
    std::cout << "\n";
    ++iterationCount;
  }
  if (!areAnyvalues(data))
  {
    std::cout << 0 << "\n";
  }
  size_t i = 0;
  if (maximumIteration)
  {
    std::cout << sumArray[i];
  }
  ++i;
  for (; i < maximumIteration; ++i)
  {
    std::cout << " " << sumArray[i];
  }
  if (maximumIteration)
  {
    std::cout << "\n";
  }
  delete [] sumArray;
}

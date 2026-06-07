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

  size_t iterationCount = 0;
  while (iterationCount < maximumIteration)
  {
    outIt = data.cbegin();
    size_t i = 0;

    for (; i < data.size(); ++i)
    {
      if (outIt->second.size() > iterationCount)
      {
        LCIter< size_t > innerIt = outIt->second.cbegin();
        innerIt += iterationCount;

        sumArray[sumIteration] += *innerIt;

        std::cout << *innerIt;
        ++outIt;
        break;
      }

      ++outIt;
    }

    ++i;
    for (; i < data.size(); ++i)
    {
      if (outIt->second.size() > iterationCount)
      {
        LCIter< size_t > innerIt = outIt->second.cbegin();
        innerIt += iterationCount;

        sumArray[sumIteration] += *innerIt;

        std::cout << " " << *innerIt;
      }

      ++outIt;
    }

    std::cout << "\n";
    ++iterationCount;
    ++sumIteration;
  }

  if (checkOnOverflow(data))
  {
    std::cerr << "Sum of elements is overflow\n";
    delete [] sumArray;
    return 1;
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
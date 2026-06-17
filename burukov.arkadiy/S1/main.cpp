#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

#include "list.hpp"

namespace
{

using pair_t = std::pair< std::string, burukov::List< unsigned long long > >;
using iter_list_t = burukov::List< burukov::LCIter< unsigned long long > >;

template< class Container >
void printContainer(std::ostream &out, const Container &container)
{
  auto it = container.cbegin();

  if (it != container.cend())
  {
    out << *it;
    ++it;

    for (; it != container.cend(); ++it)
    {
      out << " " << *it;
    }
  }
}

void printNames(std::ostream &out, const burukov::List< pair_t > &seqs)
{
  auto it = seqs.cbegin();

  if (it != seqs.cend())
  {
    out << it->first;
    ++it;

    for (; it != seqs.cend(); ++it)
    {
      out << " " << it->first;
    }
  }
}

void readSequences(std::istream &in, burukov::List< pair_t > &seqs)
{
  std::string line;
  bool hasTail = false;
  burukov::LIter< pair_t > seqTail;

  while (std::getline(in, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string name;

    if (!(iss >> name))
    {
      continue;
    }

    burukov::List< unsigned long long > numbers;
    unsigned long long value = 0;
    bool numHasTail = false;
    burukov::LIter< unsigned long long > numTail;

    while (iss >> value)
    {
      if (!numHasTail)
      {
        numbers.pushFront(value);
        numTail = numbers.begin();
        numHasTail = true;
      }
      else
      {
        numTail = numbers.insertAfter(numTail, value);
      }
    }

    pair_t p;
    p.first = name;
    p.second = std::move(numbers);

    if (!hasTail)
    {
      seqs.pushFront(std::move(p));
      seqTail = seqs.begin();
      hasTail = true;
    }
    else
    {
      seqTail = seqs.insertAfter(seqTail, std::move(p));
    }
  }
}

size_t getMaxLen(const burukov::List< pair_t > &seqs)
{
  size_t result = 0;

  for (auto it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    result = std::max(result, it->second.size());
  }

  return result;
}

void buildIterators(const burukov::List< pair_t > &seqs, iter_list_t &iters)
{
  bool hasIterTail = false;
  burukov::LIter< burukov::LCIter< unsigned long long > > iterTail;

  for (auto seqIt = seqs.cbegin(); seqIt != seqs.cend(); ++seqIt)
  {
    if (!hasIterTail)
    {
      iters.pushFront(seqIt->second.cbegin());
      iterTail = iters.begin();
      hasIterTail = true;
    }
    else
    {
      iterTail = iters.insertAfter(iterTail, seqIt->second.cbegin());
    }
  }
}

void printColumnRow(std::ostream &out, const burukov::List< pair_t > &seqs, iter_list_t &iters, size_t column,
  unsigned long long &currentSum, bool &overflow)
{
  burukov::List< unsigned long long > row;
  bool rowTailSet = false;
  burukov::LIter< unsigned long long > rowTail;
  const unsigned long long maxValue = std::numeric_limits< unsigned long long >::max();

  auto iterIt = iters.begin();

  for (auto seqIt = seqs.cbegin(); seqIt != seqs.cend(); ++seqIt, ++iterIt)
  {
    if (column < seqIt->second.size())
    {
      unsigned long long value = **iterIt;

      if (!rowTailSet)
      {
        row.pushFront(value);
        rowTail = row.begin();
        rowTailSet = true;
      }
      else
      {
        rowTail = row.insertAfter(rowTail, value);
      }

      if (!overflow)
      {
        if (currentSum > maxValue - value)
        {
          overflow = true;
        }
        else
        {
          currentSum += value;
        }
      }

      ++(*iterIt);
    }
  }

  printContainer(out, row);
}

bool processColumns(std::ostream &out, std::ostream &err, const burukov::List< pair_t > &seqs, size_t maxLen,
  burukov::List< unsigned long long > &sums)
{
  if (maxLen == 0)
  {
    return false;
  }

  bool overflow = false;
  bool sumTailSet = false;
  burukov::LIter< unsigned long long > sumTail;

  iter_list_t iters;
  buildIterators(seqs, iters);

  for (size_t column = 0; column < maxLen; ++column)
  {
    unsigned long long currentSum = 0;
    printColumnRow(out, seqs, iters, column, currentSum, overflow);
    out << "\n";

    if (!overflow)
    {
      if (!sumTailSet)
      {
        sums.pushFront(currentSum);
        sumTail = sums.begin();
        sumTailSet = true;
      }
      else
      {
        sumTail = sums.insertAfter(sumTail, currentSum);
      }
    }
  }

  if (overflow)
  {
    err << "overflow\n";
    return true;
  }

  return false;
}

}

int main()
{
  burukov::List< pair_t > sequences;
  readSequences(std::cin, sequences);

  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printNames(std::cout, sequences);
  std::cout << "\n";

  size_t maxLen = getMaxLen(sequences);

  if (maxLen == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  burukov::List< unsigned long long > sums;

  if (processColumns(std::cout, std::cerr, sequences, maxLen, sums))
  {
    return 1;
  }

  printContainer(std::cout, sums);
  std::cout << "\n";

  return 0;
}

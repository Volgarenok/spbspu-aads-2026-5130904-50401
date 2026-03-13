#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <limits>

namespace burukov
{
  using pair_t = std::pair< std::string, List< unsigned long long > >;

  void readSequences(std::istream &in, List< pair_t > &seqs)
  {
    LIter< pair_t > seqsTail;
    bool seqsHasTail = false;
    std::string name;
    while (in >> name)
    {
      List< unsigned long long > nums;
      LIter< unsigned long long > numTail;
      bool numHasTail = false;
      unsigned long long num = 0;
      while (in >> num)
      {
        if (!numHasTail)
        {
          nums.pushFront(num);
          numTail = nums.begin();
          numHasTail = true;
        }
        else
        {
          numTail = nums.insertAfter(numTail, num);
        }
      }
      in.clear();
      pair_t p;
      p.first = name;
      p.second = std::move(nums);
      if (!seqsHasTail)
      {
        seqs.pushFront(std::move(p));
        seqsTail = seqs.begin();
        seqsHasTail = true;
      }
      else
      {
        seqsTail = seqs.insertAfter(seqsTail, std::move(p));
      }
    }
  }

  void printNames(std::ostream &out, const List< pair_t > &seqs)
  {
    bool first = true;
    for (LCIter< pair_t > it = seqs.cbegin();
        it != seqs.cend(); ++it)
    {
      if (!first)
      {
        out << " ";
      }
      out << (*it).first;
      first = false;
    }
    out << "\n";
  }

  size_t getMaxLen(const List< pair_t > &seqs)
  {
    size_t maxLen = 0;
    for (LCIter< pair_t > it = seqs.cbegin();
        it != seqs.cend(); ++it)
    {
      const size_t len = (*it).second.size();
      if (len > maxLen)
      {
        maxLen = len;
      }
    }
    return maxLen;
  }

  void printRow(std::ostream &out,
      const List< unsigned long long > &row)
  {
    bool first = true;
    for (LCIter< unsigned long long > it = row.cbegin();
        it != row.cend(); ++it)
    {
      if (!first)
      {
        out << " ";
      }
      out << *it;
      first = false;
    }
    out << "\n";
  }

  int buildAndPrint(std::ostream &out, std::ostream &err,
      List< pair_t > &seqs, size_t maxLen)
  {
    List< LIter< unsigned long long > > iters;
    LIter< LIter< unsigned long long > > itersTail;
    bool itersHasTail = false;
    for (LIter< pair_t > it = seqs.begin();
        it != seqs.end(); ++it)
    {
      const LIter< unsigned long long > numIt =
          (*it).second.begin();
      if (!itersHasTail)
      {
        iters.pushFront(numIt);
        itersTail = iters.begin();
        itersHasTail = true;
      }
      else
      {
        itersTail = iters.insertAfter(itersTail, numIt);
      }
    }

    List< List< unsigned long long > > rows;
    LIter< List< unsigned long long > > rowsTail;
    bool rowsTailSet = false;

    List< unsigned long long > sums;
    LIter< unsigned long long > sumsTail;
    bool sumsTailSet = false;
    bool overflow = false;

    const unsigned long long maxVal =
        std::numeric_limits< unsigned long long >::max();

    for (size_t col = 0; col < maxLen; ++col)
    {
      List< unsigned long long > row;
      LIter< unsigned long long > rowTail;
      bool rowTailSet = false;
      unsigned long long sum = 0;

      LIter< LIter< unsigned long long > > curIt =
          iters.begin();
      for (LIter< pair_t > seqIt = seqs.begin();
          seqIt != seqs.end(); ++seqIt, ++curIt)
      {
        if (col >= (*seqIt).second.size())
        {
          continue;
        }
        const unsigned long long val = *(*curIt);
        if (!rowTailSet)
        {
          row.pushFront(val);
          rowTail = row.begin();
          rowTailSet = true;
        }
        else
        {
          rowTail = row.insertAfter(rowTail, val);
        }
        if (!overflow)
        {
          if (sum > maxVal - val)
          {
            overflow = true;
          }
          else
          {
            sum += val;
          }
        }
        ++(*curIt);
      }

      if (!rowsTailSet)
      {
        rows.pushFront(row);
        rowsTail = rows.begin();
        rowsTailSet = true;
      }
      else
      {
        rowsTail = rows.insertAfter(rowsTail, row);
      }
      if (!overflow)
      {
        if (!sumsTailSet)
        {
          sums.pushFront(sum);
          sumsTail = sums.begin();
          sumsTailSet = true;
        }
        else
        {
          sumsTail = sums.insertAfter(sumsTail, sum);
        }
      }
    }

    for (LIter< List< unsigned long long > > it =
        rows.begin(); it != rows.end(); ++it)
    {
      printRow(out, *it);
    }

    if (overflow)
    {
      err << "overflow\n";
      return 1;
    }

    bool first = true;
    for (LIter< unsigned long long > it = sums.begin();
        it != sums.end(); ++it)
    {
      if (!first)
      {
        out << " ";
      }
      out << *it;
      first = false;
    }
    out << "\n";
    return 0;
  }
}

int main()
{
  burukov::List< burukov::pair_t > seqs;
  burukov::readSequences(std::cin, seqs);
  if (seqs.empty())
  {
    std::cout << 0 << "\n";
    return 0;
  }
  burukov::printNames(std::cout, seqs);
  const size_t maxLen = burukov::getMaxLen(seqs);
  if (maxLen == 0)
  {
    return 0;
  }
  return burukov::buildAndPrint(std::cout, std::cerr, seqs, maxLen);
}
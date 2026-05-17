#include "functions.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include "list.hpp"


void malashenko::getData(std::istream& in, malashenko::List< pair_t >& res)
{
  std::string name;
  while ((in >> name) && !in.eof())
  {
    malashenko::List< size_t > nums;
    size_t num;
    while (in >> num)
    {
      nums.push_back(num);
    }
    pair_t p(name, nums);
    res.push_back(p);
    in.clear();
  }
}
size_t malashenko::getCheckedSum(size_t a, size_t b)
{
  const size_t max_st = std::numeric_limits< size_t >::max();
  if (max_st - a > b)
  {
    return a + b;
  }
  throw std::overflow_error("overflow");
}
size_t malashenko::getMaxSeqSize(const List< pair_t >& list)
{
  size_t maxSize = 0;
  for (LCIter< pair_t > s = list.begin(); s != list.end(); ++s)
  {
    maxSize = std::max(maxSize, s->second.size());
  }
  return maxSize;
}
void malashenko::printSeqNames(std::ostream& out, const List< pair_t >& list)
{
  LCIter< pair_t > s = list.begin();
  out << s->first;
  ++s;
  for (; s != list.end(); ++s)
  {
    out << ' ' << s->first;
  }
}
void malashenko::getTransedSeq(List< List< size_t > >& res, const List< pair_t >& list)
{
  size_t maxSize = getMaxSeqSize(list);
  for (size_t curSize = 1; curSize < maxSize + 1; ++curSize)
  {
    List< size_t > newSeq;
    for (LCIter< pair_t > s = list.begin(); s != list.end(); ++s)
    {
      if (s->second.size() < curSize)
      {
        continue;
      }
      LCIter< size_t > valNode = (s->second.begin()) + (curSize - 1);
      newSeq.push_back(*valNode);
    }
    res.push_back(newSeq);
  }
}
void malashenko::printNewSeqs(std::ostream& out, const List< List< size_t > >& list)
{
  for (LCIter< List< size_t > > outStart = list.begin(); outStart != list.end(); ++outStart)
  {
    LCIter< size_t > inStart = outStart->begin();
    out << *inStart;
    ++inStart;
    for (; inStart != outStart->end(); ++inStart)
    {
      out << ' ' << *inStart;
    }
    out << '\n';
  }
}
void malashenko::countSums(const List< List< size_t > >& list, List< size_t >& sums)
{
  for (LCIter< List< size_t > > outStart = list.begin(); outStart != list.end(); ++outStart)
  {
    size_t sum = 0;
    for (LCIter< size_t > inStart = outStart->begin(); inStart != outStart->end(); ++inStart)
    {
      sum = malashenko::getCheckedSum(sum, *inStart);
    }
    sums.push_back(sum);
  }
}
void malashenko::printSums(std::ostream& out, const List< size_t >& sums)
{
  if (sums.empty())
  {
    return;
  }
  LCIter< size_t > s = sums.begin();
  out << *s;
  ++s;
  for (; s != sums.end(); ++s)
  {
    out << ' ' << *s;
  }
}


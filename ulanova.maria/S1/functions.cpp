#include "functions.hpp"
#include <cstddef>
#include <cctype>
#include <limits>
#include <stdexcept>


ulanova::List< ulanova::Sequence > ulanova::read_sequences(std::istream& in)
{
  ulanova::List< ulanova::Sequence > sequences;
  std::string name;

  while (in >> name)
  {
    ulanova::Sequence seq;
    seq.name = name;

    char separator = '\0';
    while (in.get(separator) && separator != '\n')
    {
      if (separator == '\r')
      {
        continue;
      }

      size_t value = 0;
      in >> value;
      seq.values.push_back(value);
    }

    sequences.push_back(seq);
  }
  return sequences;
}

ulanova::List< ulanova::List < size_t > > ulanova::transpose_sequences(const ulanova::List< ulanova::Sequence >& seqs)
{
  ulanova::List< ulanova::List < size_t > > result;

  if (seqs.cbegin() == seqs.cend())
  {
    return result;
  }

  ulanova::List< ulanova::LCIter < size_t > > iters;

  for (ulanova::LCIter < ulanova::Sequence > it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    iters.push_back(it->values.cbegin());
  }

  bool done = false;

  while (!done)
  {
    done = true;

    ulanova::List< size_t >  row;

    ulanova::LCIter< ulanova::Sequence > seq_it = seqs.cbegin();
    ulanova::LIter< ulanova::LCIter < size_t > > iter_it = iters.begin();

    for (; seq_it != seqs.cend(); ++seq_it, ++iter_it)
    {
      ulanova::LCIter< size_t > & cur = *iter_it;

      if (cur != seq_it->values.cend())
      {
        row.push_back(*cur);
        ++cur;
        done = false;
      }
    }

    if (row.cbegin() != row.cend())
    {
      result.push_back(row);
    }
  }

  return result;
}

ulanova::List< size_t >  ulanova::calculate_sums(const ulanova::List< ulanova::List < size_t > >& seqs)
{
  ulanova::List< size_t >  sums;

  for (ulanova::LCIter< ulanova::List < size_t > > it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    size_t sum = 0;

    for (ulanova::LCIter< size_t >  jt = it->cbegin(); jt != it->cend(); ++jt)
    {
      size_t value = *jt;
      if (sum > std::numeric_limits< size_t > ::max() - value)
      {
        throw std::overflow_error("overflow");
      }
      sum += value;
    }

    sums.push_back(sum);
  }

  return sums;
}

void ulanova::print_sequences(std::ostream& out, const ulanova::List< ulanova::List < size_t > >& seqs)
{
  ulanova::LCIter< ulanova::List< size_t > > it = seqs.cbegin();

  if (it != seqs.cend())
  {
    ulanova::LCIter< size_t > jt = it->cbegin();

    if (jt != it->cend())
    {
      out << *jt;
      ++jt;
    }

    for (; jt != it->cend(); ++jt)
    {
      out << " " << *jt;
    }

    ++it;
  }

  for (; it != seqs.cend(); ++it)
  {
    out << "\n";

    ulanova::LCIter< size_t > jt = it->cbegin();

    if (jt != it->cend())
    {
      out << *jt;
      ++jt;
    }

    for (; jt != it->cend(); ++jt)
    {
      out << " " << *jt;
    }
  }
}

void ulanova::print_sums(std::ostream& out, const ulanova::List< size_t > & sums)
{
  ulanova::LCIter< size_t > it = sums.cbegin();

  if (it != sums.cend())
  {
    out << *it;
    ++it;
  }

  for (; it != sums.cend(); ++it)
  {
    out << " " << *it;
  }
}

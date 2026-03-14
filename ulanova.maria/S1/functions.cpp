#include "functions.hpp"
#include <cstddef>
#include <cctype>
#include <limits>


List<Sequence> read_sequences(std::istream& in)
{
  List<Sequence> sequences;
  std::string name;

  while (in >> name)
  {
    Sequence seq;
    seq.name = name;

    while (true)
    {
      int c = in.peek();

      if (c == EOF || c == '\n')
      {
        break;
      }

      if (std::isdigit(c))
      {
        size_t value;
        in >> value;
        seq.values.push_front(value);
      }
      else
      {
        in.get();
      }
    }

    sequences.push_front(seq);
  }

  return sequences;
}

List<List<size_t>> transpose_sequences(const List<Sequence>& seqs)
{
  List<List<size_t>> result;

  if (!(seqs.cbegin() != seqs.cend()))
  {
    return result;
  }

  List<LCIter<size_t>> iters;

  for (LCIter<Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    iters.push_front(it->values.cbegin());
  }

  bool done = false;

  while (!done)
  {
    done = true;

    List<size_t> row;

    LCIter<Sequence> seq_it = seqs.cbegin();
    LIter<LCIter<size_t>> iter_it = iters.begin();

    for (; seq_it != seqs.cend() && iter_it != iters.end(); ++seq_it, ++iter_it)
    {
      LCIter<size_t> cur = *iter_it;

      if (cur != seq_it->values.cend())
      {
        row.push_front(*cur);
        ++(*iter_it);
        done = false;
      }
    }

    if (row.cbegin() != row.cend())
    {
      List<size_t> new_row;

      for (LCIter<size_t> it = row.cbegin(); it != row.cend(); ++it)
      {
        new_row.push_front(*it);
      }

      result.push_front(new_row);
    }
  }

  return result;
}

List<size_t> calculate_sums(const List<List<size_t>>& seqs)
{
  List<size_t> sums;

  for (LCIter<List<size_t>> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    size_t sum = 0;

    for (LCIter<size_t> jt = it->cbegin(); jt != it->cend(); ++jt)
    {
      if (sum > std::numeric_limits<size_t>::max() - *jt)
      {
        std::cerr << "overflow\n";
        std::exit(1);
      }
      sum += *jt;
    }

    sums.push_front(sum);
  }

  return sums;
}

void print_sequences(const List<List<size_t>>& seqs)
{
  for (LCIter<List<size_t>> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    for (LCIter<size_t> jt = it->cbegin(); jt != it->cend(); ++jt)
    {
      std::cout << *jt << " ";
    }
    std::cout << "\n";
  }
}

void print_sums(const List<size_t>& sums)
{
  for (LCIter<size_t> it = sums.cbegin(); it != sums.cend(); ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << "\n";
}

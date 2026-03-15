#include "functions.hpp"
#include <cstddef>
#include <cctype>
#include <limits>

namespace ulanova
{
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
          seq.values.push_back(value);
        }
        else
        {
          in.get();
        }
      }

      sequences.push_back(seq);
    }

    return sequences;
  }

  List<List<size_t>> transpose_sequences(const List<Sequence>& seqs)
  {
    List<List<size_t>> result;

    if (seqs.cbegin() == seqs.cend())
    {
      return result;
    }

    List<LCIter<size_t>> iters;

    for (LCIter<Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
    {
      iters.push_back(it->values.cbegin());
    }

    bool done = false;

    while (!done)
    {
      done = true;

      List<size_t> row;

      LCIter<Sequence> seq_it = seqs.cbegin();
      LIter<LCIter<size_t>> iter_it = iters.begin();

      for (; seq_it != seqs.cend(); ++seq_it, ++iter_it)
      {
        LCIter<size_t>& cur = *iter_it;

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

  List<size_t> calculate_sums(const List<List<size_t>>& seqs)
  {
    List<size_t> sums;

    for (LCIter<List<size_t>> it = seqs.cbegin(); it != seqs.cend(); ++it)
    {
      size_t sum = 0;

      for (LCIter<size_t> jt = it->cbegin(); jt != it->cend(); ++jt)
      {
        size_t value = *jt;
        if (sum > std::numeric_limits<size_t>::max() - value)
        {
          std::cerr << "overflow\n";
          std::exit(1);
        }
        sum += value;
      }

      sums.push_back(sum);
    }

    return sums;
  }

  void print_sequences(const List<List<size_t>>& seqs)
  {
    for (LCIter<List<size_t>> it = seqs.cbegin(); it != seqs.cend(); ++it)
    {
      bool first = true;

      for (LCIter<size_t> jt = it->cbegin(); jt != it->cend(); ++jt)
      {
        if (!first)
        {
          std::cout << " ";
        }

        std::cout << *jt;
        first = false;
      }

      std::cout << "\n";
    }
  }
  void print_sums(const List<size_t>& sums)
  {
    bool first = true;

    for (LCIter<size_t> it = sums.cbegin(); it != sums.cend(); ++it)
    {
      if (!first)
      {
        std::cout << " ";
      }

      std::cout << *it;
      first = false;
    }

    std::cout << "\n";
  }
}

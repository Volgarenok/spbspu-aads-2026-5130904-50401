#include "functions.hpp"

List<Sequence> read_sequences(std::istream& in)
{
  List<Sequence> sequences;
  std::string name;
  while(in >> name)
  {
    Sequence seq;
    seq.name = name;
    int value;
    while (in >> value)
    {
      seq.values.push_front(value);
      if (in.peek() == '\n')
      {
        break;
      }
    }
    sequences.push_front(seq);
  }
  return sequences;
}
List<List<int>> transpose_sequences(const List<Sequence>& seqs)
{
  List<List<int>> result;
  List<LCIter<int>> iters;
  for (LCIter<Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    iters.push_front(it -> values.cbegin());
  }
  bool done = false;
  while (!done)
  {
    done = true;
    LCIter<Sequence> seq_it = seqs.cbegin();
    LIter<LCIter<int>> iter_it = iters.begin();
    for (; seq)
  }
}

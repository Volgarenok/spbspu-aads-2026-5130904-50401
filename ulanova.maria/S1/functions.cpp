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


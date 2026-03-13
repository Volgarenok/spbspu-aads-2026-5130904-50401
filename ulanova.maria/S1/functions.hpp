#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <string>
#include "list.hpp"

struct Sequence
{
  std::string name;
  List<int> values;
};
List<Sequence> read_sequences(std::istream& in);
List<List<int>> transpose_sequences(const List<Sequence>& seqs);


#endif

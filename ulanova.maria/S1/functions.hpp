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


#endif
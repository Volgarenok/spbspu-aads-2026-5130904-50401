#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include <iostream>

#include "List.hpp"

namespace khasnulin
{
  std::pair< std::string, BiList< int > > readRow(std::istream &in)
  {
    std::string name;
    BiList< int > list;
    in >> name;
    int curr_value = 0;
    while (in >> curr_value)
    {
      list.push_back(curr_value);
    }
    if (!in.eof() && in.fail())
    {
      in.clear();
    }
    return std::pair< std::string, BiList< int > >{name, list};
  }

  BiList< std::pair< std::string, BiList< int > > > readAll(std::istream &in)
  {
  }
}

#endif

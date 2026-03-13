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

  using bilist_row_pairs = BiList< std::pair< std::string, BiList< int > > >;
  bilist_row_pairs readAll(std::istream &in)
  {
    bilist_row_pairs result;
    while (!in.eof() && !in.fail())
    {
      std::pair< std::string, BiList< int > > row = readRow(in);
      if (row.first != "")
      {
        result.push_back(row);
      }
    }
    return result;
  }
}

#endif

#include "Sequences.hpp"

#include <iostream>

std::pair< std::string, khasnulin::BiList< int > > khasnulin::readRow(std::istream &in)
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

using bilist_row_pairs = khasnulin::BiList< std::pair< std::string, khasnulin::BiList< int > > >;
bilist_row_pairs khasnulin::readAll(std::istream &in)
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

void khasnulin::printSequenceNames(std::ostream &out, bilist_row_pairs &sequence)
{
  bool first = true;
  for (const auto &item : sequence)
  {
    if (!first)
    {
      out << " ";
    }
    first = false;
    out << item.first;
  }
  out << "\n";
}
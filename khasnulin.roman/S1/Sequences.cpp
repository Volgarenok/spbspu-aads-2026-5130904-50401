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

khasnulin::bilist_row_pairs khasnulin::readAll(std::istream &in)
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

void khasnulin::printSequenceNames(std::ostream &out, const bilist_row_pairs &sequence)
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

void khasnulin::printSequencesNumsByPlace(std::ostream &out, const bilist_row_pairs &sequence)
{
  BiList< LCIter< int > > it_list;
  for (const auto &item : sequence)
  {
    it_list.push_back(item.second.begin());
  }

  bool all_empty = false;
  while (!all_empty)
  {
    all_empty = true;
    bool first = true;
    auto curr_it = it_list.begin();
    for (const auto &curr_seq : sequence)
    {
      if (*curr_it != curr_seq.second.end())
      {
        if (!first)
        {
          out << " ";
        }
        out << *(*curr_it);
        ++(*curr_it);
        all_empty = false;
        first = false;
      }
      ++curr_it;
    }
    if (!all_empty)
    {
      out << "\n";
    }
  }
}
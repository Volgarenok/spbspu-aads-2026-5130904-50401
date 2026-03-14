#include "Sequences.hpp"

#include <fcntl.h>
#include <iostream>
#include <limits>
#include <stdexcept>

int khasnulin::safeSum(int a, int b)
{
  static constexpr int maxInt = std::numeric_limits< int >::max();
  static constexpr int minInt = std::numeric_limits< int >::min();
  if (b > 0 && a > maxInt - b)
  {
    throw std::overflow_error("numers sum overflow");
  }
  if (b < 0 && a < minInt - b)
  {
    throw std::underflow_error("numers sum underflow");
  }
  return a + b;
}

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

khasnulin::BiList< khasnulin::BiList< int > >
khasnulin::getTransosedNumsSequences(const bilist_row_pairs &sequence)
{
  BiList< LCIter< int > > it_list;
  for (const auto &item : sequence)
  {
    it_list.push_back(item.second.begin());
  }

  BiList< BiList< int > > answer;
  bool all_empty = false;
  while (!all_empty)
  {
    all_empty = true;
    auto curr_it = it_list.begin();
    BiList< int > current_layer;
    for (const auto &curr_seq : sequence)
    {
      if (*curr_it != curr_seq.second.end())
      {
        current_layer.push_back(*(*curr_it));
        ++(*curr_it);
        all_empty = false;
      }
      ++curr_it;
    }
    if (!all_empty)
    {
      answer.push_back(std::move(current_layer));
    }
  }
  return answer;
}

void khasnulin::printSequencesNumsByPlace(std::ostream &out, const BiList< BiList< int > > &sequence)
{
  for (const BiList< int > &seq : sequence)
  {
    bool first = true;
    for (const int num : seq)
    {
      if (!first)
      {
        out << " ";
      }
      out << num;
      first = false;
    }
    out << "\n";
  }
}

void khasnulin::printSumsOfSequences(std::ostream &out, const BiList< BiList< int > > &sequence)
{
  bool first = true;
  for (const BiList< int > &seq : sequence)
  {
    int sum = 0;
    for (int num : seq)
    {
      sum = safeSum(sum, num);
    }
    if (!first)
    {
      out << " ";
    }
    out << sum;
    first = false;
  }
  out << "\n";
}
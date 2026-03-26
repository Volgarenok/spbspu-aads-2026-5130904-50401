#include "Sequences.hpp"

#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

size_t khasnulin::safeSum(size_t a, size_t b)
{
  static constexpr size_t maxInt = std::numeric_limits< size_t >::max();
  if (b > 0 && a > maxInt - b)
  {
    throw std::overflow_error("numbers sum overflow");
  }
  return a + b;
}

std::pair< std::string, khasnulin::BiList< size_t > > khasnulin::readRow(std::istream &in)
{
  std::string name;
  BiList< size_t > list;
  in >> name;
  size_t curr_value = 0;
  while (in >> curr_value)
  {
    list.push_back(curr_value);
  }
  if (!in.eof() && in.fail())
  {
    in.clear();
  }
  return std::pair< std::string, BiList< size_t > >{name, list};
}

khasnulin::bilist_row_pairs khasnulin::readAll(std::istream &in)
{
  bilist_row_pairs result;
  while (!in.eof() && !in.fail())
  {
    std::pair< std::string, BiList< size_t > > row = readRow(in);
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

khasnulin::BiList< khasnulin::BiList< size_t > >
khasnulin::getTransosedNumsSequences(const bilist_row_pairs &sequence)
{
  BiList< LCIter< size_t > > it_list;
  for (const auto &item : sequence)
  {
    it_list.push_back(item.second.begin());
  }

  BiList< BiList< size_t > > answer;
  bool all_empty = false;
  while (!all_empty)
  {
    all_empty = true;
    auto curr_it = it_list.begin();
    BiList< size_t > current_layer;
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

void khasnulin::printSequencesNumsByPlace(std::ostream &out, const BiList< BiList< size_t > > &sequence)
{
  for (const BiList< size_t > &seq : sequence)
  {
    bool first = true;
    for (const size_t num : seq)
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

khasnulin::BiList< size_t > khasnulin::getListOfSums(const BiList< BiList< size_t > > &sequence)
{
  BiList< size_t > sums;
  for (const BiList< size_t > &seq : sequence)
  {
    size_t sum = 0;
    for (size_t num : seq)
    {
      sum = safeSum(sum, num);
    }
    sums.push_back(sum);
  }
  return sums;
}

void khasnulin::printSumsOfSequences(std::ostream &out, const BiList< BiList< size_t > > &sequence)
{
  BiList< size_t > sums = getListOfSums(sequence);
  bool first = true;
  for (const size_t &sum : sums)
  {
    if (!first)
    {
      out << " ";
    }
    out << sum;
    first = false;
  }
  out << "\n";
}

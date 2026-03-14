#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <string>
#include <cstddef>
#include "list.hpp"

struct Sequence
{
  std::string name;
  List<size_t> values;
};
List<Sequence> read_sequences(std::istream& in);
List<size_t> copy_list(const List<size_t>& src);
List<List<size_t>> transpose_sequences(const List<Sequence>& seqs);
List<size_t> calculate_sums(const List<List<size_t>>& seqs);
void print_sequences(const List<List<size_t>>& seqs);
void print_sums(const List<size_t>& sums);
#endif

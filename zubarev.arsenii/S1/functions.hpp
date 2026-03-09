#ifndef FUNC_HPP
#define FUNC_HPP
#include <iosfwd>
#include "data.hpp"
namespace zubarev
{
  List< Data > input(std::istream& in);
  size_t max_sequences(List< Data >* list);
  void output_names(List< Data >* list);
  void output_sequences(List< Data >* list);
  void output_sums(List< Data >* list);
}

#endif

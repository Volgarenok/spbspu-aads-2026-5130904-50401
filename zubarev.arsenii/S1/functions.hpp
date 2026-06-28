#ifndef FUNC_HPP
#define FUNC_HPP
#include <iosfwd>
#include "../common/my_list/data.hpp"

namespace zubarev
{
  List< Data > input(std::istream& in, bool& error);
  size_t max_sequences(const List< Data >& list);
  size_t output_names(const List< Data >& list);
  size_t output_sequences(const List< Data >& list);
  List< size_t > calculate_sums(const List< Data >& list);
  void print_sums(const List< size_t >& list);

}

#endif

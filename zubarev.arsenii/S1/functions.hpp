#ifndef FUNC_HPP
#define FUNC_HPP
#include <iosfwd>
#include "data.hpp"
namespace zubarev
{
  List< Data > input(std::istream& in,bool& error);
  template < class T >
  void output(std::ostream& out, T el, bool is_first);

  size_t max_sequences(List< Data >* list);
  int output_names(List< Data >* list);
  int output_sequences(List< Data >* list);
  int output_sums(List< Data >* list);
}

#endif

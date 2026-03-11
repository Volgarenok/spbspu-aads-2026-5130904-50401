#ifndef FUNC_HPP
#define FUNC_HPP
#include <iosfwd>
#include "data.hpp"
namespace zubarev
{
  List< Data > input(std::istream& in);
  template <class T>
  void output(std::ostream &out, T el, bool is_first);

  size_t max_sequences(List< Data >* list);
  void output_names(List< Data >* list);
  void output_sequences(List< Data >* list);
  void output_sums(List< Data >* list);
}

#endif

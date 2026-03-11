#ifndef FUNC_HPP
#define FUNC_HPP
#include <iosfwd>
#include "data.hpp"
namespace zubarev
{
  List< Data > input(std::istream& in, bool& error);
  template < class T >
  void output(std::ostream& out, T el, bool is_first);

  size_t max_sequences(List< Data >* list);
  size_t output_names(List< Data >* list);
  size_t output_sequences(List< Data >* list);
  size_t output_sums(List< Data >* list);
  size_t to_size_t(const std::string& num);
  bool isdigit(const std::string& str);

}

#endif

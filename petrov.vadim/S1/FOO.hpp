#ifndef FOO_HPP
#define FOO_HPP
#include "STRUCTS.hpp"
#include <string>
namespace petrov
{
  void mkData(std::istream& input, List< std::pair< std::string, List< size_t > > >& data);
  void printAllList(List< std::pair< std::string, List< size_t > > >& data);
  bool canShowItem(const List< size_t >& nums, size_t istr, std::istream& out);
  size_t maxLen(const List< std::pair< std::string, List< size_t > > >& data);
  LIter< size_t > showSums(List< size_t >& nums, std::istream& out);
}
#endif

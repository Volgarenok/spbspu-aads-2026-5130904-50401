#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include "../common/List/list_struct.hpp"
namespace petrov
{
  void mkData(std::istream& input, List< std::pair< std::string, List< size_t > > >& data);
  void printAllList(List< std::pair< std::string, List< size_t > > >& data, std::ostream& out);
  bool canShowItem(const List< size_t >& nums, size_t istr);
  size_t maxLen(const List< std::pair< std::string, List< size_t > > >& data);
  void showSums(List< size_t >& nums, std::ostream& out);
}
#endif

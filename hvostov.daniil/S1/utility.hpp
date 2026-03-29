#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <string>
#include "list.hpp"

namespace hvostov {
  List< std::pair< std::string, List< size_t > > > getData(std::istream& in);
  size_t fromStringToNumber(const std::string& str);
  void printInfo(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list);
  void printResult(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list);
  bool isCorrectNumber(const std::string& str);
  size_t sumNumbersWithOverflow(size_t a, size_t b);
}

#endif

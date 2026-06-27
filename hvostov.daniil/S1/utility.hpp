#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <string>
#include <list.hpp>

namespace hvostov {
  size_t sumNumbersWithOverflow(size_t a, size_t b);

  List< std::pair< std::string, List< size_t > > > getData(std::istream& in);
  List< LCiter< size_t > > createIteratorList(const List< std::pair< std::string, List< size_t > > >& list);
  List< size_t > calculateSums(const List< std::pair< std::string, List< size_t > > >& list);

  void printNames(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list);
  void printNumbers(std::ostream& out, List< LCiter< size_t > >& list_it);
  void printList(std::ostream& out, const List< size_t >& list);
  void printInfo(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list);
  void printResult(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list);
}

#endif

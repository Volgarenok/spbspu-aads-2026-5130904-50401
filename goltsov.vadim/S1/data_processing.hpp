#ifndef DATA_PROCESSING_HPP
#define DATA_PROCESSING_HPP
#include <string>
#include <mylist.hpp>
namespace goltsov
{
  size_t sumWithCheck(size_t a, size_t b);
  void getData(std::istream& in, List< std::pair< std::string, List< size_t > > >& result, size_t& size);
  void push_back(size_t** sums, size_t& n, size_t a);
  std::ostream& printResult(std::ostream& out, goltsov::List< std::pair< std::string,
    goltsov::List< size_t > > >& data, size_t size);
  std::ostream& printResultNamesAndMakeListOfIterators(std::ostream& out, goltsov::List< std::pair< std::string,
    goltsov::List< size_t > > >& data, List< LIter< size_t > >& list_its);
  std::ostream& printResultNumbersAndSums(std::ostream& out, List< LIter< size_t > >& list_its);
}
#endif

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "mylist.hpp"
#include <string>
namespace goltsov
{
  size_t sumWithCheck(size_t a, size_t b);
  bool isCorrectNumber(const std::string& num);
  size_t fromStringToST(const std::string& num);
  void getData(List< std::pair< std::string, List< size_t > > >& result, std::istream& in, size_t& size);
  void push_back(size_t** sums, size_t& n, size_t a);
  std::ostream& printResult(std::ostream& a, goltsov::List< std::pair< std::string,
    goltsov::List< size_t > > >& data, size_t size);
}
#endif

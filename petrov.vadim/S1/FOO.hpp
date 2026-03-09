#ifndef FOO_HPP
#define FOO_HPP
#include "STRUCTS.hpp"
#include <string>
namespace petrov
{
  void mkData(std::istream& input, List< std::pair< std::string, size_t > >& data);
}
#endif // !FOO_HPP
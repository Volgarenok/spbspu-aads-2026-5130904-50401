#ifndef YALOVSKY_F0_INPUT_HPP
#define YALOVSKY_F0_INPUT_HPP

#include <iosfwd>
#include <string>

namespace yalovsky
{
  std::string readToken(std::istream& input);
  unsigned long long readUnsigned(std::istream& input);
  void requireEnd(std::istream& input);
}

#endif

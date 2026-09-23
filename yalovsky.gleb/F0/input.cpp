#include "input.hpp"
#include <istream>
#include <limits>
#include <stdexcept>

std::string yalovsky::readToken(std::istream& input)
{
  std::string token;
  if (!(input >> token))
  {
    throw std::invalid_argument("Missing argument");
  }
  return token;
}

unsigned long long yalovsky::readUnsigned(std::istream& input)
{
  const std::string token = readToken(input);
  const auto maximum = std::numeric_limits< unsigned long long >::max();
  constexpr unsigned long long base = 10;
  unsigned long long value = 0;

  for (char symbol: token)
  {
    if (symbol < '0' || symbol > '9')
    {
      throw std::invalid_argument("Invalid unsigned integer");
    }

    const auto digit = static_cast< unsigned long long >(symbol - '0');
    if (value > (maximum - digit) / base)
    {
      throw std::out_of_range("Integer is too large");
    }

    value = value * base + digit;
  }

  return value;
}

void yalovsky::requireEnd(std::istream& input)
{
  input >> std::ws;
  if (input.bad() || !input.eof())
  {
    throw std::invalid_argument("Unexpected argument");
  }
}

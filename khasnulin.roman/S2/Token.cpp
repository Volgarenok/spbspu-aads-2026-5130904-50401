#include "Token.hpp"

#include <limits>
#include <stdexcept>

namespace
{
  int strToNum(const std::string &str)
  {
    int max_int = std::numeric_limits< int >::max();
    int res = 0;
    for (char ch : str)
    {
      if (ch < '0' || ch > '9')
      {
        throw std::runtime_error("get not digit char while trying to read number from string");
      }
      int digit = ch - '0';
      if (res <= (max_int - digit) / 10)
      {
        res = res * 10 + digit;
      }
      else
      {
        throw std::overflow_error("get overflow while reading number from string");
      }
    }
    return res;
  }
}

khasnulin::Token khasnulin::readToken(std::istream &in)
{
  std::string str;
  Token newToken;
  if (in >> str && str.length() > 0)
  {
    if (str[0] >= '0' && str[0] <= '9')
    {
      newToken = {TokenType::Number, strToNum(str), '0', 0};
    }
    else if (str[0] == '(')
    {
      newToken = {TokenType::LeftParen, 0, '(', 0};
    }
    else if (str[0] == ')')
    {
      newToken = {TokenType::RightParen, 0, ')', 0};
    }
    else if (str.length() == 1 &&
             (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/' || str[0] == '%'))
    {
      newToken = {TokenType::Operator, 0, str[0], getOpPriority(str[0])};
    }
    else
    {
      throw std::runtime_error("read unknown token from input stream");
    }
  }
  else
  {
    newToken = {TokenType::End, 0, '0', 0};
  }

  return newToken;
}

int khasnulin::getOpPriority(char ch)
{
  switch (ch)
  {
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
  case '%':
    return 2;
  default:
    return -1;
  }
}

std::ostream &khasnulin::operator<<(std::ostream &out, const TokenType &type)
{
  return out << "token type: " << static_cast< int >(type) << " ";
}

#include "Token.hpp"

#include <limits>
#include <stdexcept>
#include <string>

namespace
{
  long long strToNum(const std::string &str)
  {
    long long max_int = std::numeric_limits< long long >::max();
    long long res = 0;
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

  bool isOperationLexeme(const std::string &lexeme)
  {
    return lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "%" ||
           lexeme == ">>";
  }
}

khasnulin::Token khasnulin::readToken(const std::string &line, size_t &pos)
{
  size_t size = line.length();
  Token newToken;

  while (pos < size && std::isspace(line[pos]))
  {
    pos++;
  }

  if (pos == size)
  {
    newToken = {TokenType::EndOfExpr, 0, '0', 0};
    return newToken;
  }

  std::string lexeme;
  while (pos < size && line[pos] != ' ')
  {
    lexeme += line[pos++];
  }

  if ((lexeme[0] >= '0' && lexeme[0] <= '9'))
  {
    newToken = {TokenType::Number, strToNum(lexeme), '0', 0};
  }
  else if (lexeme.length() == 1 && lexeme[0] == '(')
  {
    newToken = {TokenType::LeftParen, 0, '(', 0};
  }
  else if (lexeme.length() == 1 && lexeme[0] == ')')
  {
    newToken = {TokenType::RightParen, 0, ')', 0};
  }
  else if (isOperationLexeme(lexeme))
  {
    newToken = {TokenType::Operator, 0, lexeme[0], getOpPriority(lexeme[0])};
  }
  else
  {
    throw std::runtime_error("read unknown token from input stream");
  }

  return newToken;
}

int khasnulin::getOpPriority(char ch)
{
  switch (ch)
  {
  case '>':
    return 100;
  case '+':
  case '-':
    return 200;
  case '*':
  case '/':
  case '%':
    return 300;
  default:
    return -1;
  }
}

std::ostream &khasnulin::operator<<(std::ostream &out, const TokenType &type)
{
  return out << "token type: " << static_cast< int >(type) << " ";
}

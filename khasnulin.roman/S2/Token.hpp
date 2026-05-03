#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <ostream>

namespace khasnulin
{
  enum class TokenType : char
  {
    Number,
    Operator,
    LeftParen,
    RightParen,
    EndOfExpr
  };

  std::ostream &operator<<(std::ostream &out, const TokenType &type);

  struct Token
  {
    TokenType type;
    long long value;
    char op;
    int priority;
  };

  Token readToken(const std::string &line, size_t &pos);

  int getOpPriority(char ch);
}

#endif

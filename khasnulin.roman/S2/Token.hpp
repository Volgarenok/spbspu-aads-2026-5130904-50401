#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <istream>
#include <ostream>

namespace khasnulin
{
  enum class TokenType : char
  {
    Number,
    Operator,
    LeftParen,
    RightParen,
    End
  };

  std::ostream &operator<<(std::ostream &out, const TokenType &type);

  struct Token
  {
    TokenType type;
    int value;
    char op;
    int priority;
  };

  Token readToken(std::istream &in);

  int getOpPriority(char ch);
}

#endif

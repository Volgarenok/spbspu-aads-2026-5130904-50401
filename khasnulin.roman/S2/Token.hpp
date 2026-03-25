#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <istream>

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

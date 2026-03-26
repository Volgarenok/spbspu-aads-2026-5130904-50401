#include "ExpressionProcess.hpp"
#include "Queue.hpp"
#include "Token.hpp"
#include <string>

khasnulin::Queue< khasnulin::Token > khasnulin::readExpression(std::istream &in)
{
  std::string line;
  size_t pos = 0;
  std::getline(in, line);
  Queue< Token > result;
  Token lastToken;
  lastToken = readToken(line, pos);
  while (lastToken.type != TokenType::EndOfExpr)
  {
    result.push(lastToken);
    lastToken = readToken(line, pos);
  }
  return result;
}

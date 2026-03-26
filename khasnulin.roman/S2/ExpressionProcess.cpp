#include "ExpressionProcess.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "Token.hpp"
#include <string>
#include <utility>

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

khasnulin::Stack< khasnulin::Queue< khasnulin::Token > > khasnulin::readAllExpressions(std::istream &in)
{
  Stack< Queue< Token > > result;
  while (!in.eof() && !in.fail())
  {
    Queue< Token > expression = readExpression(in);
    if (!expression.empty())
    {
      result.push(std::move(expression));
    }
  }
  return result;
}

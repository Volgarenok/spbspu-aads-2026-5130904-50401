#include "ExpressionProcess.hpp"

namespace
{
  int calculateBinaryOp(int v1, int v2, char op)
  {
    switch (op)
    {
    case '+':
      return v2 + v1;
    case '-':
      return v2 - v1;
    case '*':
      return v2 * v1;
    case '/':
      return v2 / v1;
    case '%':
      return v2 % v1;
    default:
      return v2;
    }
  }

  void makeSingleCalculation(khasnulin::Stack< khasnulin::Token > &values,
                             khasnulin::Stack< khasnulin::Token > &operations)
  {
    char op = operations.top().op;
    operations.pop();
    int v1 = values.top().value;
    values.pop();
    int v2 = values.top().value;
    values.pop();
    int result = calculateBinaryOp(v1, v2, op);
    values.push(khasnulin::Token{khasnulin::TokenType::Number, result, '0', 0});
  }
}

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

int khasnulin::readAndProcessExpressionLine(const std::string &line)
{
  size_t pos = 0;
  Stack< Token > values;
  Stack< Token > operations;
  Token lastToken = readToken(line, pos);

  while (lastToken.type != TokenType::EndOfExpr)
  {
    processToken(lastToken, values, operations);
    lastToken = readToken(line, pos);
  }

  while (!operations.empty())
  {
    makeSingleCalculation(values, operations);
  }

  return values.top().value;
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

void khasnulin::processToken(Token &token, Stack< Token > &values, Stack< Token > &operations)
{
  if (token.type == TokenType::Number)
  {
    values.push(token);
  }
  if (token.type == TokenType::LeftParen)
  {
    operations.push(token);
  }
  else if (token.type == TokenType::Operator)
  {
    while (!operations.empty() && token.priority <= operations.top().priority)
    {
      makeSingleCalculation(values, operations);
    }
    operations.push(token);
  }
  else if (token.type == TokenType::RightParen)
  {
    while (!operations.empty() && operations.top().type != TokenType::LeftParen)
    {
      makeSingleCalculation(values, operations);
    }
    operations.pop();
  }
}

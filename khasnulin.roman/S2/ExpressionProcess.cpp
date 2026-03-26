#include "ExpressionProcess.hpp"

#include <istream>
#include <limits>
#include <stdexcept>
#include <string>

namespace
{
  int safeSum(int a, int b)
  {
    static constexpr int max = std::numeric_limits< int >::max();
    static constexpr int min = std::numeric_limits< int >::min();
    if (b > 0 && a > max - b)
    {
      throw std::overflow_error("get overflow while sum");
    }
    if (b < 0 && a < min - b)
    {
      throw std::underflow_error("get underflow while sum");
    }
    return a + b;
  }

  int safeSubtraction(int a, int b)
  {
    static constexpr int max = std::numeric_limits< int >::max();
    static constexpr int min = std::numeric_limits< int >::min();
    if (b > 0 && a < min + b)
    {
      throw std::underflow_error("get underflow while subtract");
    }
    if (b < 0 && a > max + b)
    {
      throw std::overflow_error("get overflow while subtract");
    }
    return a - b;
  }

  int safeDivide(int a, int b)
  {
    static constexpr int min = std::numeric_limits< int >::min();
    if (b == -1 && a == min)
    {
      throw std::underflow_error("get underflow while divide (get int min / (-1))");
    }
    if (b == 0)
    {
      throw std::logic_error("can't divide to zero");
    }
    return a / b;
  }

  int safeMultiply(int a, int b)
  {
    static constexpr int max = std::numeric_limits< int >::max();
    static constexpr int min = std::numeric_limits< int >::min();
    if ((a > 0 && b > 0 && a > max / b) || (a < 0 && b < 0 && a < max / b))
    {
      throw std::overflow_error("get overflow while multyplying");
    }
    if ((a > 0 && b < 0 && b < min / a) || (a < 0 && b > 0 && a < min / b))
    {
      throw std::underflow_error("get underflow while myltiplying");
    }
    return a * b;
  }

  int safeModulo(int a, int b)
  {
    static constexpr int min = std::numeric_limits< int >::min();
    if (a == min && b == -1)
    {
      throw std::overflow_error("get overflow while module operation");
    }
    if (b == 0)
    {
      throw std::logic_error("can't get modulo of zero");
    }
    return a % b;
  }

  int calculateBinaryOp(int v1, int v2, char op)
  {
    switch (op)
    {
    case '+':
      return safeSum(v2, v1);
    case '-':
      return safeSubtraction(v2, v1);
    case '*':
      return safeMultiply(v2, v1);
    case '/':
      return safeDivide(v2, v1);
    case '%':
      return safeModulo(v2, v1);
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
  return !values.empty() ? values.top().value : 0;
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
  else if (token.type == TokenType::LeftParen)
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

khasnulin::Stack< int > khasnulin::readAndProcessAllExpressions(std::istream &in)
{
  Stack< int > answers;
  std::string line;
  while (std::getline(in, line))
  {
    bool empty = true;
    for (size_t i = 0, end = line.size(); i < end && empty; ++i)
    {
      if (!std::isspace(line[i]))
      {
        empty = false;
      }
    }
    if (!empty)
    {
      int result = readAndProcessExpressionLine(line);
      answers.push(result);
    }
  }
  return answers;
}

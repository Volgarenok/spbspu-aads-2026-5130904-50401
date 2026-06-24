#include "functions.hpp"

#include <limits>
#include <stdexcept>

#include "stack.hpp"

long long tarasenko::calculate(const std::string& line)
{
  auto input_queue = stringToQueue(line);
  Stack< long long > operands_stack;
  Stack< Token > operations_stack;
  while (!input_queue.empty())
  {
    Token cur = stringToToken(input_queue.front());
    input_queue.pop();
    if (cur.type == TokenType::num)
    {
      operands_stack.push(cur.value);
    }
    else if (cur.type == TokenType::left_parenthe)
    {
      operations_stack.push(cur);
    }
    else if (cur.type == TokenType::right_parenthe)
    {
      while (!operations_stack.empty() && operations_stack.top().type != TokenType::left_parenthe)
      {
        makeTopOperation(operations_stack, operands_stack);
      }
      if (operations_stack.empty())
      {
        throw std::logic_error("Incorrect expression");
      }
      operations_stack.pop();
    }
    else if (cur.type == TokenType::operation)
    {
      while (!operations_stack.empty() && cur.priority <= operations_stack.top().priority)
      {
        makeTopOperation(operations_stack, operands_stack);
      }
      operations_stack.push(cur);
    }
  }

  while (!operations_stack.empty())
  {
    if (operations_stack.top().type == TokenType::left_parenthe)
    {
      throw std::logic_error("Incorrect expression");
    }
    makeTopOperation(operations_stack, operands_stack);
  }

  if (!(operands_stack.size() == 1))
  {
    throw std::logic_error("Incorrect expression");
  }
  long long tmp = operands_stack.top();
  operands_stack.pop();
  return tmp;
}

tarasenko::Queue< std::string > tarasenko::stringToQueue(const std::string& line)
{
  Queue< std::string > queue;
  size_t i = 0;
  while (i < line.size())
  {
    if (line[i] == ' ')
    {
      ++i;
    }
    size_t start = i;
    while (i < line.size() && line[i] != ' ')
    {
      ++i;
    }
    if (start != i)
    {
      queue.push(line.substr(start, i - start));
    }
  }
  return queue;
}

tarasenko::Token tarasenko::stringToToken(const std::string& str)
{
  if (str.empty())
  {
    throw std::logic_error("Incorrect input");
  }
  if (str == "(")
  {
    return Token{TokenType::left_parenthe, 0, 0, 0};
  }
  if (str == ")")
  {
    return Token{TokenType::right_parenthe, 0, 0, 0};
  }
  if (isOperation(str))
  {
    return Token{TokenType::operation, 0, str[0], getPriority(str)};
  }
  size_t pos = 0;
  long long num = 0;
  try
  {
    num = std::stoll(str, &pos);
  }
  catch (const std::invalid_argument&)
  {
    throw std::logic_error("Incorrect input");
  }
  if (pos != str.size())
  {
    throw std::logic_error("Incorrect input");
  }
  return Token{TokenType::num, num, 0, 0};
}

bool tarasenko::isOperation(const std::string& str)
{
  return str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == ">>";
}

int tarasenko::getPriority(const std::string& str)
{
  if (str == "+" || str == "-")
  {
    return 1;
  }
  if (str == "*" || str == "/" || str == "%")
  {
    return 2;
  }
  if (str == ">>")
  {
    return 3;
  }
  throw std::logic_error("Incorrect operation");
}

long long tarasenko::add(long long a, long long b)
{
  if ((b > 0 && a > MAX - b) || (b < 0 && a < MIN - b))
  {
    throw std::overflow_error("addition overflow");
  }
  return a + b;
}

long long tarasenko::subtract(long long a, long long b)
{
  if ((b > 0 && a < MIN + b) || (b < 0 && a > MAX + b))
  {
    throw std::overflow_error("subtraction overflow");
  }
  return a - b;
}

long long tarasenko::multiply(long long a, long long b)
{
  bool pos_overflow = a > 0 && b > 0 && a > MAX / b;
  bool neg_overflow = a < 0 && b < 0 && a < MAX / b;
  bool pos_neg_overflow = a > 0 && b < 0 && b < MIN / a;
  bool neg_pos_overflow = a < 0 && b > 0 && a < MIN / b;
  if (pos_overflow || neg_overflow || pos_neg_overflow || neg_pos_overflow)
  {
    throw std::overflow_error("multiplying overflow");
  }
  return a * b;
}

long long tarasenko::divide(long long a, long long b)
{
  if (a == MIN && b == -1)
  {
    throw std::overflow_error("division overflow");
  }
  if (b == 0)
  {
    throw std::logic_error("division by zero");
  }
  return a / b;
}

long long tarasenko::mod(long long a, long long b)
{
  if (a == MIN && b == -1)
  {
    throw std::overflow_error("division overflow");
  }
  if (b == 0)
  {
    throw std::logic_error("division by zero");
  }
  long long res = a % b;
  return res >= 0 ? res : b + res;
}

long long tarasenko::rightShift(long long a, long long b)
{
  const int LEN = std::numeric_limits< long long >::digits;
  if (b < 0)
  {
    throw std::invalid_argument("negative shift");
  }
  if (b >= LEN)
  {
    throw std::invalid_argument("too large shift");
  }
  return a >> b;
}

void tarasenko::makeTopOperation(tokenStack& operations_stack, numStack& operands_stack)
{
  if (operations_stack.empty())
  {
    throw std::logic_error("Empty operations stack");
  }
  Token operation = operations_stack.top();
  operations_stack.pop();

  if (operands_stack.size() < 2)
  {
    throw std::logic_error("Empty operands stack");
  }
  long long right_operand = operands_stack.top();
  operands_stack.pop();

  long long left_operand = operands_stack.top();
  operands_stack.pop();
  long long result;
  switch (operation.operation)
  {
    case '+':
      result = add(left_operand, right_operand);
      break;
    case '-':
      result = subtract(left_operand, right_operand);
      break;
    case '*':
      result = multiply(left_operand, right_operand);
      break;
    case '/':
      result = divide(left_operand, right_operand);
      break;
    case '%':
      result = mod(left_operand, right_operand);
      break;
    case '>':
      result = rightShift(left_operand, right_operand);
      break;
    default:
      throw std::logic_error("Incorrect operation");
  }
  operands_stack.push(result);
}

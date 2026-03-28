#include "functions.hpp"
#include "stack.hpp"
#include <stdexcept>
#include <limits>

const long long MAX = std::numeric_limits< long long >::max();
const long long MIN = std::numeric_limits< long long >::min();

namespace tarasenko
{
  long long calculate(std::string line)
  {
    auto input_queue = stringToQueue(line);
    Stack< long long > operands_stack;
    Stack< Token > operations_stack;
    while(!input_queue.empty())
    {
      Token cur = input_queue.pop();
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
    return operands_stack.pop();
  }

  Queue< Token > stringToQueue(const std::string& line)
  {
    Queue< Token > queue;
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (std::isdigit(line[i]))
      {
        long long num = getNumber(line, i);
        queue.push(Token{TokenType::num, num, 0, 0});
        --i;
      }
      else
      {
        switch (line[i])
        {
          case ' ':
            break;
          case '(':
            queue.push(Token{TokenType::left_parenthe, 0, 0, 0});
            break;
          case ')':
            queue.push(Token{TokenType::right_parenthe, 0, 0, 0});
            break;
          case '+':
            queue.push(Token{TokenType::operation, 0, '+', 1});
            break;
          case '-':
            queue.push(Token{TokenType::operation, 0, '-', 1});
            break;
          case '*':
            queue.push(Token{TokenType::operation, 0, '*', 2});
            break;
          case '/':
            queue.push(Token{TokenType::operation, 0, '/', 2});
            break;
          case '%':
            queue.push(Token{TokenType::operation, 0, '%', 2});
            break;
          case '>':
           if (i + 1 >= line.size() || line[i + 1] != '>')
            {
              throw std::logic_error("Incorrect input");
            }
            queue.push(Token{TokenType::operation, 0, '>', 3});
            ++i;
            break;
          default:
            throw std::logic_error("Incorrect input");
        }
      }
    }
    return queue;
  }

  long long getNumber(const std::string& line, size_t& pos)
  {
    long long num = 0;
    for (; pos < line.size(); ++pos)
    {
      if (std::isdigit(line[pos]))
      {
        if (num > (MAX - (line[pos] - '0')) / 10)
        {
          throw std::overflow_error("number overflow");
        }
        num = num * 10 + (line[pos] - '0');
      }
      else
      {
        break;
      }
    }
    return num;
  }

  long long add(long long a, long long b)
  {
    if ((b > 0 && a > MAX - b) || (b < 0 && a < MIN - b))
    {
      throw std::overflow_error("addition overflow");
    }
    return a + b;
  }

  long long subtract(long long a, long long b)
  {
    if ((b > 0 && a < MIN + b) || (b < 0 && a > MAX + b))
    {
      throw std::overflow_error("subtraction overflow");
    }
    return a - b;
  }

  long long multiply(long long a, long long b)
  {
    if ((a > 0 && b > 0 && a > MAX / b) || (a < 0 && b < 0 && a < MAX / b) \
    || (a > 0 && b < 0 && b < MIN / a) || (a < 0 && b > 0 && a < MIN / b))
    {
      throw std::overflow_error("multiplying overflow");
    }
    return a * b;
  }

  long long divide(long long a, long long b)
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

  long long mod(long long a, long long b)
  {
    if (a == MIN && b == -1)
    {
      throw std::overflow_error("division overflow");
    }
    if (b == 0)
    {
      throw std::logic_error("division by zero");
    }
    return a % b;
  }

  long long rightShift(long long a, long long b)
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

  void makeTopOperation(tokenStack& operations_stack, numStack& operands_stack)
  {
    if (operations_stack.empty())
    {
      throw std::logic_error("Empty operations stack");
    }
    Token operation = operations_stack.pop();

    if (operands_stack.empty())
    {
      throw std::logic_error("Empty operands stack");
    }
    long long right_operand = operands_stack.pop();

    if (operands_stack.empty())
    {
      throw std::logic_error("Empty operands stack");
    }
    long long left_operand = operands_stack.pop();
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
}

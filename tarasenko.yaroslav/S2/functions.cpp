#include "functions.hpp"
#include "stack.hpp"
#include <stdexcept>

namespace tarasenko
{
  long long calculate(std::string line)
  {
    auto input_queue = stringToQueue(line);
    Stack< Token > operand_stack;
    Stack< Token > operations_stack;
    /*while(!input_queue.empty())
    {
      Token cur = input_queue.pop();
    }*/
    return 0;
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
            queue.push(Token{TokenType::operation, 0, '+', 0});
            break;
          case '-':
            queue.push(Token{TokenType::operation, 0, '-', 0});
            break;
          case '*':
            queue.push(Token{TokenType::operation, 0, '*', 1});
            break;
          case '/':
            queue.push(Token{TokenType::operation, 0, '/', 1});
            break;
          case '%':
            queue.push(Token{TokenType::operation, 0, '%', 1});
            break;
          case '>':
            queue.push(Token{TokenType::operation, 0, '>', 2});
            ++i;
            break;
          default:
            throw std::runtime_error("Incorrect input");
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
        num = num * 10 + (line[pos] - '0');
      }
      else
      {
        break;
      }
    }
    return num;
  }
}

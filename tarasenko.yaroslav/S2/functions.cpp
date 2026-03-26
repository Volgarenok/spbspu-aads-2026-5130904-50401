#include "functions.hpp"
#include "stack.hpp"

namespace tarasenko
{
  long long calculate(std::string line)
  {
    auto input_queue = stringToQueue(line);
    Stack< long long > operand_stack;
    Stack< char > operations_stack;
    while(!input_queue.empty())
    {
      char cur = input_queue.pop();
      if (cur == '(')
      {
        operations_stack.push(cur);
      }
    }
    return 0;
  }

  Queue< char > stringToQueue(std::string line)
  {
    Queue< char > queue;
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] == ' ')
      {
        continue;
      }
      if (isdigit(line[i]))
      {
        queue.push(getNumber(line, i));
      }
      else
      {
        queue.push(line[i]);
      }
    }
    return queue;
  }

  long long getNumber(std::string line, size_t& pos)
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

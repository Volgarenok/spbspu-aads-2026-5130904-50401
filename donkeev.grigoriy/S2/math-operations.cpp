#include <fstream>
#include <limits>
#include "stack.hpp"
#include "queue.hpp"

using llint_t = long long int;

namespace donkeev
{
  void readFromFile(Queue< Queue< char > >& expressionsQueue, const char* filename)
  {
    std::ifstream input(filename);
    if (!input)
    {
      throw std::runtime_error("Can't open the file");
    }

    char test;
    while (input >> test)
    {
      if (test == ' ' || test == '\n')
      {
        continue;
      }
      if (input.eof())
      {
        break;
      }

      donkeev::Queue< char > symbolsQueue;
      symbolsQueue.push(test);
      input >> test;
      while (test != '\n')
      {
        if (input.eof())
        {
          break;
        }
        if (test != ' ')
        {
          symbolsQueue.push(test);
        }
        input >> test;
      }

      expressionsQueue.push(symbolsQueue);
    }
  }
  void readFromTerminal(Queue< Queue< char > >& expressionsQueue, std::istream& input)
  {
    char test;
    while (input >> test)
    {
      if (test == ' ' || test == '\n')
      {
        continue;
      }
      if (input.eof())
      {
        break;
      }

      donkeev::Queue< char > symbolsQueue;
      symbolsQueue.push(test);
      input >> test;
      while (test != '\n')
      {
        if (input.eof())
        {
          break;
        }
        if (test != ' ')
        {
          symbolsQueue.push(test);
        }
        input >> test;
      }

      expressionsQueue.push(symbolsQueue);
    }
  }

  bool isNumber(const Queue< char >& q, llint_t &operand)
  {
    operand = 0;
    Queue<char> temp = q;
    
    while (!temp.isEmpty())
    {
      char ch = temp.front();
      temp.pop();

      if (!std::isdigit(static_cast<unsigned char>(ch)))
      {
        return false;
      }
      if ((operand * 10 + (ch - '0')) < operand)
      {
        throw std::overflow_error("Number overflow");
      }

      operand = operand * 10 + (ch - '0');
    }
    return true;
  }
  bool isOperator(const char& ch)
  {
    const char operators[] = {'+', '-', '*', '/', '%'};
    const size_t operatorsSize = 5;

    for (size_t i = 0; i < operatorsSize; ++i)
    {
      if (ch == operators[i])
      {
        return true;
      }
    }

    return false;
  }
  size_t getPriority(const char& ch)
  {
    if (ch == '+' || ch == '-')
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
  llint_t doOperation(const llint_t& operand1, const llint_t& operand2, const char& operation)
  {
    llint_t max_llint = std::numeric_limits< llint_t >::max();
    llint_t min_llint = std::numeric_limits< llint_t >::min();

    if (operation == '+')
    {
      if ((operand2 > 0 && operand1 > max_llint - operand2) || (operand2 < 0 && operand1 < min_llint - operand2))
      {
        throw std::overflow_error("Summ overflow");
      }

      return operand1 + operand2;
    }
    else if (operation == '-')
    {
      if ((operand1 < 0 && operand1 < min_llint + operand2) || (operand2 < 0 && operand1 > max_llint + operand2))
      {
        throw std::overflow_error("Subtraction overflow");
      }
    }
    
  }
  void calculate(Stack< llint_t >& result, Queue< Queue< char > >& expressionsQueue)
  {
    while (!expressionsQueue.isEmpty())
    {
      Queue< char > innerQueue = expressionsQueue.front();
      Stack< char > stack;
      Queue< llint_t > finishQueue;

      char test;
      while (!innerQueue.isEmpty())
      {
        test = innerQueue.front();
        if (test == '(')
        {
          stack.push(test);
          innerQueue.pop();
        }
        else if (test >= '0' && test <= '9')
        {
          Queue< char > charQueue;
          charQueue.push(test);
          innerQueue.pop();
          if (!innerQueue.isEmpty())
          {
            test = innerQueue.front();
            while (test != ' ' || !innerQueue.isEmpty())
            {
              charQueue.push(test);
              innerQueue.pop();
            }
          }
          else
          {
            finishQueue.push(test - '0');
            result.push(finishQueue.front());
            break;
          }
          
          llint_t operand;
          if (isNumber(charQueue, operand))
          {
            finishQueue.push(operand);
          }
          else
          {
            throw std::invalid_argument("Invalid expression");
          }
        }
        else if (isOperator(test))
        {
          while (!(getPriority(test) < getPriority(stack.top())))
          {
            if (finishQueue.size() == 2)
            {
              llint_t operand2 = finishQueue.front();
              finishQueue.pop();
              llint_t operand1 = finishQueue.front();
              finishQueue.pop();
              try
              {
                finishQueue.push(doOperation(operand1, operand2, test));
              }
              catch ()
              {

              }
            }
          }
        }
      }
    }
  }
}

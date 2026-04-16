#include <fstream>
#include <limits>
#include <iomanip>
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

    bool isSkipws = input.flags() & std::ios_base::skipws;
    if (isSkipws)
    {
      input >> std::noskipws;
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

    if(isSkipws)
    {
      input >> std::skipws;
    }
  }
  void readFromTerminal(Queue< Queue< char > >& expressionsQueue, std::istream& input)
  {
    bool isSkipws = input.flags() & std::ios_base::skipws;
    if (isSkipws)
    {
      input >> std::noskipws;
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

    if(isSkipws)
    {
      input >> std::skipws;
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

      return operand1 - operand2;
    }
    else if (operation == '*')
    {
      if (operand1 != 0 && operand2 != 0)
      {
        if ((operand1 > max_llint / operand2) || (operand1 < min_llint / operand2))
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }

      return operand1 * operand2;
    }
    else if (operation == '/')
    {
      if (operand2 == 0)
      {
        throw std::domain_error("Division by zero");
      }
      if (operand1 == min_llint && operand2 == -1)
      {
        throw std::overflow_error("Division overflow");
      }

      return operand1 / operand2;
    }
    else if (operation == '%')
    {
      if (operand2 == 0)
      {
        throw std::domain_error("Modulo by zero");
      }
      if (operand1 == std::numeric_limits<llint_t>::min() && operand2 == -1)
      {
        throw std::overflow_error("Modulo overflow");
      }

      return operand1 % operand2;
    }
    else
    {
      throw std::invalid_argument("Unknown operator");
    }
  }
  void calculate(Stack< llint_t >& result, Queue< Queue< char > >& expressionsQueue)
  {
    while (!expressionsQueue.isEmpty())
    {
      Queue< char > innerQueue = expressionsQueue.front();
      Stack< char > operatorStack;
      Stack< llint_t > finishStack;

      char test;
      while (!innerQueue.isEmpty())
      {
        test = innerQueue.front();
        innerQueue.pop();
        if (test == ' ')
        {
          continue;
        }
        if (test == '(')
        {
          operatorStack.push(test);
        }
        else if (std::isdigit(static_cast<unsigned char>(test)))
        {
          Queue< char > charQueue;
          charQueue.push(test);
          while (!innerQueue.isEmpty() && std::isdigit(static_cast<unsigned char>(innerQueue.front())))
          {
            charQueue.push(innerQueue.front());
            innerQueue.pop();
          }

          llint_t operand;
          if (!isNumber(charQueue, operand))
          {
            throw std::invalid_argument("Invalid expression");
          }
          finishStack.push(operand);
        }
        else if (isOperator(test))
        {
          while (!operatorStack.isEmpty() && operatorStack.top() != '(' && getPriority(test) <= getPriority(operatorStack.top()))
          {
            if (finishStack.size() < 2)
            {
              throw std::invalid_argument("Invalid expression");
            }

            llint_t operand2 = finishStack.top();
            finishStack.pop();
            llint_t operand1 = finishStack.top();
            finishStack.pop();
            finishStack.push(doOperation(operand1, operand2, operatorStack.top()));
            operatorStack.pop();
          }

          operatorStack.push(test);
        }
        else if (test == ')')
        {
          while(!operatorStack.isEmpty() && operatorStack.top() != '(')
          {
            if (finishStack.size() < 2)
            {
              throw std::invalid_argument("Invalid expression");
            }

            llint_t operand2 = finishStack.top();
            finishStack.pop();
            llint_t operand1 = finishStack.top();
            finishStack.pop();
            finishStack.push(doOperation(operand1, operand2, operatorStack.top()));
            operatorStack.pop();
          }

          operatorStack.pop();
        }
        else
        {
          throw std::invalid_argument("Invalid expression");
        }
      }

      while (!operatorStack.isEmpty())
      {
        if (finishStack.size() < 2)
        {
          throw std::invalid_argument("Invalid expression");
        }

        llint_t operand2 = finishStack.top();
        finishStack.pop();
        llint_t operand1 = finishStack.top();
        finishStack.pop();
        finishStack.push(doOperation(operand1, operand2, operatorStack.top()));
        operatorStack.pop();
      }

      if (finishStack.size() != 1)
      {
        throw std::invalid_argument("Invalid expression");
      }

      result.push(finishStack.top());
      expressionsQueue.pop();
    }
  }

  std::ostream& printResult(Stack< llint_t >& result, std::ostream& out)
  {
    if (!result.isEmpty())
    {
      out << result.top();
      result.pop();
    }
    while (!result.isEmpty())
    {
      out << " " << result.top();
      result.pop();
    }
    out << '\n';
    
    return out;
  }
}

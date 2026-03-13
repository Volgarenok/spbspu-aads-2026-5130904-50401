#include "math_functions.hpp"

namespace malashenko
{
  bool isOperand(const std::string& symbol)
  {
    std::string notOperands[8] = {"+", "-", "/", "*", "%", "##"};
    for (size_t i = 0; i < 9; ++i)
    {
      if (symbol == notOperands[i])
      {
        return false;
      }
    }
    return true;
  }

  size_t getPriority(const std::string& symbol)
  {
    if (symbol == "##")
    {
      return 0;
    } else if (symbol == "+" || symbol == "-")
    {
      return 1;
    } else
    {
      return 2;
    }
  }


  void getInfixData(std::istream& in, Queue< std::string >& infixData)
  {
    std::string symbol;
    while ((in >> symbol) && !in.eof())
    {
      infixData.push(symbol);
    }
  }

  void converInfixToPostfix(Queue< std::string > infixData, Queue< std::string >& PostfixData)
  {
    Stack< std::string > stack;
    while (!infixData.empty())
    {
      std::string symbol = infixData.front();
      infixData.pop();

      if (symbol == "(")
      {
        stack.push(symbol);
        // std::cout << "pushed (\n";
        // std::cout << "stack: ";
        // stack.show();
        continue;
      }

      if (symbol == ")")
      {
        // std::cout << "stack: ";
        // stack.show();
        while (stack.top() != "(")
        {
          PostfixData.push(stack.top());
          stack.pop();
        }
        stack.pop();
        continue;
      }

      if (isOperand(symbol))
      {
        PostfixData.push(symbol);
        // std::cout << "pushed " << symbol << "\n";
      } else 
      {
        if (stack.empty() || stack.top() == "(" || stack.top() == ")")
        {
          stack.push(symbol);
          continue;
        }
        if (getPriority(stack.top()) >= getPriority(symbol))
        {

          PostfixData.push(stack.top());
          stack.pop();
          // std::cout << "queue: ";
          // PostfixData.show();
          stack.push(symbol);

          continue;
        }

      }
    }
    while (!stack.empty())
    {
      PostfixData.push(stack.top());
      stack.pop();
    }
  }

  lli_t calculate(const Queue< std::string >& PostfixData)
  {
    
  }



}


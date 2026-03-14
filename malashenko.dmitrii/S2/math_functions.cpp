#include "math_functions.hpp"

namespace malashenko
{
  bool isOperand(const std::string& symbol)
  {
    std::string notOperands[6] = {"+", "-", "/", "*", "%", "##"};
    for (size_t i = 0; i < 7; ++i)
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

  lli_t addition(const lli_t& a, const lli_t& b)
  {
    lli_t maxllt = std::numeric_limits< lli_t >::max();
    lli_t minllt = std::numeric_limits< lli_t >::min();

    if ((b > 0 && a > maxllt - b) || (b < 0 && a < minllt - b))
    {
      throw std::overflow_error("Addition overflow");
    }
    
    return a + b;
  }

  lli_t subtraction(const lli_t& a, const lli_t& b)
  {
    lli_t maxllt = std::numeric_limits< lli_t >::max();
    lli_t minllt = std::numeric_limits< lli_t >::min();

    if ((b > 0 && a < maxllt + b) || (b < 0 && a > minllt + b))
    {
      throw std::overflow_error("Subtraction overflow");
    }

    return a - b;
  }

  lli_t multiplication(const lli_t& a, const lli_t& b)
  {
    lli_t maxllt = std::numeric_limits< lli_t >::max();
    lli_t minllt = std::numeric_limits< lli_t >::min();

    if (a == 0 || b == 0)
    {
      return 0;
    }

    if (a == -1)
    {
      if (b == minllt)
      {
        throw std::overflow_error("Multiplication overflow");
      }
      return -b;
    }

    if (b == -1)
    {
      if (a == minllt)
      {
        throw std::overflow_error("Multiplication overflow");
      }
      return -a;
    }

    if (a > 0)
    {
      if (b > 0)
      {
        if (a > maxllt / b)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
      else
      {
        if (b < minllt / a)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }
    else
    {
      if (b > 0)
      {
        if (a < minllt / b)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
      else
      {
        if (a != 0 && b < maxllt / a)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
    }

    return a * b;
  }

  lli_t division(const lli_t& a, const lli_t& b)
  {
    lli_t minllt = std::numeric_limits< lli_t >::min();

    if (b == 0)
    {
      throw std::invalid_argument("Division by zero");
    }

    if (a == minllt && b == -1)
    {
      throw std::overflow_error("Division overflow");
    }

    return a / b;
  }

  lli_t concatenation(const lli_t& a, const lli_t& b)
  {
    lli_t b_copy = b;
    lli_t a_copy = a;

    if (b == 0)
    {
      lli_t res;
      try
      {
        res = multiplication(a, 10);
      } catch (const std::overflow_error&)
      {
        throw std::overflow_error("Concatenation overflow");
      }
      return res;
    }

    while (b_copy)
    {
      try
      {
        a_copy = multiplication(a_copy, 10);
      } catch (const std::overflow_error&)
      {
        throw std::overflow_error("Concatenation overflow");
      }
      b_copy /= 10;
    }

    lli_t res;
    try
    {
      res = addition(a_copy, b);
    }
    catch (const std::overflow_error &)
    {
      throw std::overflow_error("Concatenation overflow");
    }
    return res;
  }

  lli_t modulo(const lli_t & a, const lli_t &b)
  {
    lli_t minllt = std::numeric_limits< lli_t >::min();

    if (b == 0)
    {
      throw std::domain_error("Modulo by zero");
    }

    if (a == minllt && b == -1)
    {
      throw std::overflow_error("Modulo overflow");
    }

    return a % b;
  }


  size_t getIndex(const std::string& sign, const List< std::string >& funcs)
  {
    size_t ind = 0;
    for (LIter< std::string > it = funcs.begin(); it != funcs.end(); ++it, ++ind)
    {
      if ((*it) == sign)
      {
        return ind;
      }
    }
    return funcs.size();
  }


  void pushAndMove(Queue< std::string >& data, const std::string& value)
  {
    data.push(value);

    for (size_t i = 0; i < data.size(); ++i)
    {
      data.push(data.front());
      data.pop();
    }
    
  }

#if 1
  lli_t calculate(const Queue< std::string >& PostfixData)
  {
    List< std::string > funcNames;
    try
    {
      funcNames.push_back("+");
      funcNames.push_back("-");
      funcNames.push_back("*");
      funcNames.push_back("/");
      funcNames.push_back("%");
      funcNames.push_back("##");
    }
    catch(...)
    {
      funcNames.clear();
      throw;
    }
    

    List< func_t > funcs;
    try
    {
      funcs.push_back(addition);
      funcs.push_back(subtraction);
      funcs.push_back(multiplication);
      funcs.push_back(division);
      funcs.push_back(modulo);
      funcs.push_back(concatenation);
    }
    catch(...)
    {
      funcs.clear();
      throw;
    }

    Queue< std::string > res = PostfixData;

    while (res.size() != 1)
    {
      std::string num1 = res.front();
      res.pop();
      std::cout << "num1: " << num1 << '\n';
      std::string num2 = res.front();
      res.pop();

      std::cout << "num2: " << num2 << '\n';

      std::string operation = res.front();
      res.pop();

      std::cout << "op: " << operation << '\n';

      if (!isOperand(num1))
      {
        pushAndMove(res, num1);
        continue;
      }

      if (!isOperand(num2))
      {
        
        res.push(num1);
        res.push(operation);
        res.push(num2);
        // res.show();
        continue;
      }


      if (isOperand(operation))
      {
        pushAndMove(res, num1);
        num1 = num2;
        num2 = operation;
        operation = res.front();
        res.pop();
      }



      lli_t num1_lli;
      try
      {
        num1_lli = std::stoll(num1);
      }
      catch(const std::invalid_argument&)
      {
        funcNames.clear();
        funcs.clear();
        throw std::invalid_argument("Input error");
      }

      lli_t num2_lli;
      try
      {
        num2_lli = std::stoll(num2);
      }
      catch(const std::invalid_argument&)
      {
        funcNames.clear();
        funcs.clear();
        throw std::invalid_argument("Input error");
      }

      size_t ind = getIndex(operation, funcNames);
      if (ind == funcNames.size())
      {
        funcNames.clear();
        funcs.clear();
        throw std::invalid_argument("Unknown operation");
      }

      lli_t newNum = (*(funcs.begin() + ind))(num1_lli, num2_lli);

      std::string newNumStr = std::to_string(newNum);
      pushAndMove(res, newNumStr);


      res.show();
    }

    return std::stoll(res.front());
  }
#endif

}


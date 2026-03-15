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

  void getInfixData(std::istream& in, Stack< Queue< std::string > >& infixData)
  {
    char ch;
    std::string currentToken;
    malashenko::Queue<std::string> equation;

    while (in.get(ch))
    {
        if (ch == '\n')
        {
            if (!currentToken.empty())
            {
                equation.push(currentToken);
                currentToken.clear();
            }

            if (!equation.empty())
            {
                infixData.push(equation);
                equation.clear();
            }
        }
        else if (ch == ' ' || ch == '\t')
        {
            if (!currentToken.empty())
            {
                equation.push(currentToken);
                currentToken.clear();
            }
        }
        else
        {
            currentToken += ch;
        }
    }

    if (!currentToken.empty())
    {
        equation.push(currentToken);
    }
    if (!equation.empty())
    {
        infixData.push(equation);
    }
  }

  void converInfixToPostfix(const Queue< std::string >& infixDataOriginal, Queue< std::string >& PostfixData)
  {
    Queue< std::string > infixData = infixDataOriginal;
    Stack< std::string > stack;
    while (!infixData.empty())
    {
      std::string symbol = infixData.front();
      infixData.pop();

      if (symbol == "(")
      {
        stack.push(symbol);
        continue;
      }

      if (symbol == ")")
      {

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

  std::string calculate(const Queue< std::string >& PostfixDataOriginal)
  {
    Queue< std::string > PostfixData = PostfixDataOriginal;
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
      funcNames.clear();
      throw;
    }

    Stack< std::string > nums;

    while (!PostfixData.empty())
    {
      std::string symbol = PostfixData.front();
      PostfixData.pop();
      if (isOperand(symbol))
      {
        nums.push(symbol);
      } else
      {
        size_t ind = getIndex(symbol, funcNames);

        lli_t num1_lli;
        try
        {
          num1_lli = std::stoll(nums.top());
          nums.pop();
        }
        catch(const std::invalid_argument&)
        {
          funcNames.clear();
          funcs.clear();
          nums.clear();
          throw std::invalid_argument("Input error. Wrong operand");
        }

        lli_t num2_lli;
        try
        {
          num2_lli = std::stoll(nums.top());
          nums.pop();
        }
        catch(const std::invalid_argument&)
        {
          funcNames.clear();
          funcs.clear();
          nums.clear();
          throw std::invalid_argument("Input error. Wrong operand");
        }

        lli_t newNum = (*(funcs.begin() + ind))(num2_lli, num1_lli);
        nums.push(std::to_string(newNum));
      }
    }
    if (nums.size() != 1)
    {
      funcNames.clear();
      funcs.clear();
      nums.clear();
      throw std::invalid_argument("Input error. Wrong operation");
    }
    return nums.top();
  }
}

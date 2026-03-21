#include "math_func.hpp"
#include <climits>

namespace sedov
{
  bool isOperand(const std::string & sym)
  {
    std::string notOp[6] = {"+", "-", "*", "/", "%", ">>"};
    for (size_t i = 0; i < 6; ++i)
    {
      if (sym == notOp[i])
      {
        return false;
      }
    }
    return true;
  }

  size_t getPriority(const std::string & sym)
  {
    if (sym == ">>")
    {
      return 0;
    }
    else if (sym == "+" || sym == "-")
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }

  lli_t add(const lli_t & a, const lli_t & b)
  {
    lli_t maxllt = std::numeric_limits< lli_t >::max();
    lli_t minllt = std::numeric_limits< lli_t >::min();
    if ((b > 0 && a > maxllt - b) || (b < 0 && a < minllt - b))
    {
      throw std::overflow_error("Add overflow");
    }
    return a + b;
  }

  lli_t sub(const lli_t & a, const lli_t & b)
  {
    lli_t maxllt = std::numeric_limits< lli_t >::max();
    lli_t minllt = std::numeric_limits< lli_t >::min();
    if ((b > 0 && a < minllt + b) || (b < 0 && a > maxllt + b))
    {
      throw std::overflow_error("Sub overflow");
    }
    return a - b;
  }

  lli_t mult(const lli_t & a, const lli_t & b)
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
        throw std::overflow_error("Mult overflow");
      }
      return -b;
    }
    if (b == -1)
    {
      if (a == minllt)
      {
        throw std::overflow_error("Mult overflow");
      }
      return -a;
    }
    if (a > 0)
    {
      if (b > 0)
      {
        if (a > maxllt / b)
        {
          throw std::overflow_error("Mult overflow");
        }
      }
      else
      {
        if (b < minllt / a)
        {
          throw std::overflow_error("Mult overflow");
        }
      }
    }
    else
    {
      if (b > 0)
      {
        if (a < minllt / b)
        {
          throw std::overflow_error("Mult overflow");
        }
      }
      else
      {
        if (a != 0 && b < maxllt / a)
        {
          throw std::overflow_error("Mult overflow");
        }
      }
    }
    return a * b;
  }

  lli_t div(const lli_t & a, const lli_t & b)
  {
    lli_t minllt = std::numeric_limits< lli_t >::min();
    if (b == 0)
    {
      throw std::invalid_argument("Div by 0");
    }
    if (a == minllt && b == -1)
    {
      throw std::overflow_error("Div overflow");
    }
    return a / b;
  }

  lli_t mod(const lli_t & a, const lli_t & b)
  {
    lli_t minllt = std::numeric_limits< lli_t >::min();
    if (b == 0)
    {
      throw std::domain_error("Mod by 0");
    }
    if (a == minllt && b == -1)
    {
      throw std::overflow_error("Mod overflow");
    }
    lli_t r = a % b;
    r += (r < 0) ? std::abs(b) : 0;
    return r;
  }

  lli_t bitRightShift(const lli_t & a, const lli_t & b)
  {
    if (b < 0)
    {
      throw std::invalid_argument("Bit Right Shift by 0");
    }
    const int totalBits = sizeof(lli_t) * CHAR_BIT;
    if (b >= totalBits)
    {
      throw std::out_of_range("Out of range bits of number");
    }
    if (a == 0)
    {
      return 0;
    }
    return a >> b;
  }

  void getInfix(std::istream & in, Stack< Queue< std::string > > & infix)
  {
    char c;
    std::string cur;
    sedov::Queue< std::string > e;
    while (in.get(c))
    {
      if (c == '\n')
      {
        if (!cur.empty())
        {
          e.push(cur);
          cur.clear();
        }
        if (!e.empty())
        {
          infix.push(e);
          e.clear();
        }
      }
      else if (c == ' ' || c == '\t')
      {
        if (!cur.empty())
        {
          e.push(cur);
          cur.clear();
        }
      }
      else
      {
        cur += c;
      }
    }
    if (!cur.empty())
    {
      e.push(cur);
    }
    if (!e.empty())
    {
      infix.push(e);
    }
  }

  void convertInfToPost(const Queue< std::string > & infix, Queue< std::string > & postfix)
  {
    Queue< std::string > infixNew = infix;
    Stack< std::string > stack;
    while (!infixNew.empty())
    {
      std::string sym = infixNew.front();
      infixNew.pop();
      if (sym == "(")
      {
        stack.push(sym);
        continue;
      }
      if (sym == ")")
      {
        while (stack.top() != "(")
        {
          postfix.push(stack.top());
          stack.pop();
        }
        stack.pop();
        continue;
      }
      if (isOperand(sym))
      {
        postfix.push(sym);
      }
      else
      {
        if (stack.empty() || stack.top() == "(" || stack.top() == ")")
        {
          stack.push(sym);
          continue;
        }
        if (getPriority(stack.top()) >= getPriority(sym))
        {
          postfix.push(stack.top());
          stack.pop();
          stack.push(sym);
          continue;
        }
      }
    }
    while (!stack.empty())
    {
      postfix.push(stack.top());
      stack.pop();
    }
  }

  size_t getIndex(const std::string & sign, List< std::string > & funcs)
  {
    size_t i = 0;
    for (LIter< std::string > it = funcs.begin(); it != funcs.end(); ++it, ++i)
    {
      if ((*it) == sign)
      {
        return i;
      }
    }
    return funcs.size();
  }

  std::string calculate(const Queue< std::string > & postfix)
  {
    Queue< std::string > postfixNew = postfix;
    List< std::string > funcNames;
    try
    {
      funcNames.pushBack("+");
      funcNames.pushBack("-");
      funcNames.pushBack("*");
      funcNames.pushBack("/");
      funcNames.pushBack("%");
      funcNames.pushBack(">>");
    }
    catch (...)
    {
      funcNames.clear();
      throw;
    }

    List< func_t > funcs;
    try
    {
      funcs.pushBack(add);
      funcs.pushBack(sub);
      funcs.pushBack(mult);
      funcs.pushBack(div);
      funcs.pushBack(mod);
      funcs.pushBack(bitRightShift);
    }
    catch (...)
    {
      funcs.clear();
      funcNames.clear();
      throw;
    }

    Stack< std::string > nums;
    while (!postfixNew.empty())
    {
      std::string sym = postfixNew.front();
      postfixNew.pop();
      if (isOperand(sym))
      {
        nums.push(sym);
      }
      else
      {
        size_t ind = getIndex(sym, funcNames);
        lli_t num1;
        try
        {
          num1 = std::stoll(nums.top());
          nums.pop();
        }
        catch (const std::invalid_argument &)
        {
          funcNames.clear();
          funcs.clear();
          nums.clear();
          throw std::invalid_argument("Input error");
        }
        lli_t num2;
        try
        {
          num2 = std::stoll(nums.top());
          nums.pop();
        }
        catch (const std::invalid_argument &)
        {
          funcNames.clear();
          funcs.clear();
          nums.clear();
          throw std::invalid_argument("Input error");
        }
        auto it = funcs.begin();
        for (size_t i = 0; i < ind; ++i) {
            ++it;
        }
        lli_t newNum = (*it)(num2, num1);
        nums.push(std::to_string(newNum));
      }
    }
    if (nums.size() != 1)
    {
      funcNames.clear();
      funcs.clear();
      nums.clear();
      throw std::invalid_argument("Input error");
    }
    return nums.top();
  }
}

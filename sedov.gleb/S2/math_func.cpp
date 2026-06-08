#include "math_func.hpp"
#include <climits>

namespace
{
  bool isOperation(const std::string & sym)
  {
    return sym == "+" || sym == "-" || sym == "*" || sym == "/" || sym == "%" || sym == ">>";
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

  sedov::func_t getFunction(const std::string & sign, const sedov::List< std::string > & funcNames,
    const sedov::List< sedov::func_t > & funcs)
  {
    size_t i = 0;
    for (sedov::LCIter< std::string > it = funcNames.cbegin(); it != funcNames.cend(); ++it, ++i)
    {
      if ((*it) == sign)
      {
        auto fit = funcs.cbegin();
        for (size_t j = 0; j < i; ++j)
        {
          ++fit;
        }
        return *fit;
      }
    }
    return nullptr;
  }
}

sedov::lli_t sedov::add(const lli_t & a, const lli_t & b)
{
  lli_t maxllt = std::numeric_limits< lli_t >::max();
  lli_t minllt = std::numeric_limits< lli_t >::min();
  if ((b > 0 && a > maxllt - b) || (b < 0 && a < minllt - b))
  {
    throw std::overflow_error("Add overflow");
  }
  return a + b;
}

sedov::lli_t sedov::sub(const lli_t & a, const lli_t & b)
{
  lli_t maxllt = std::numeric_limits< lli_t >::max();
  lli_t minllt = std::numeric_limits< lli_t >::min();
  if ((b > 0 && a < minllt + b) || (b < 0 && a > maxllt + b))
  {
    throw std::overflow_error("Sub overflow");
  }
  return a - b;
}

sedov::lli_t sedov::mult(const lli_t & a, const lli_t & b)
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

sedov::lli_t sedov::div(const lli_t & a, const lli_t & b)
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

sedov::lli_t sedov::mod(const lli_t & a, const lli_t & b)
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

sedov::lli_t sedov::bitRightShift(const lli_t & a, const lli_t & b)
{
  if (b < 0)
  {
    throw std::invalid_argument("Bit Right Shift by negative");
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

void sedov::getInfix(std::istream & in, Stack< Expression > & infix)
{
  char c;
  std::string cur;
  Expression e;
  while (in.get(c))
  {
    if (c == '\n')
    {
      if (!cur.empty())
      {
        e.addToken(cur);
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
        e.addToken(cur);
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
    e.addToken(cur);
  }
  if (!e.empty())
  {
    infix.push(e);
  }
}

sedov::Expression sedov::convertInfToPost(const Expression & infix)
{
  const Queue< std::string > & infixTokens = infix.getTokens();
  Queue< std::string > infixNew = infixTokens;
  Expression result;
  Stack< std::string > stack;
  while (!infixNew.empty())
  {
    std::string sym = infixNew.front();
    infixNew.pop();
    if (sym == "(")
    {
      stack.push(sym);
    }
    else if (sym == ")")
    {
      while (stack.top() != "(")
      {
        result.addToken(stack.top());
        stack.pop();
      }
      stack.pop();
    }
    else if (isOperation(sym))
    {
      while (!stack.empty() && stack.top() != "(" && getPriority(stack.top()) >= getPriority(sym))
      {
        result.addToken(stack.top());
        stack.pop();
      }
      stack.push(sym);
    }
    else
    {
      result.addToken(sym);
    }
  }
  while (!stack.empty())
  {
    result.addToken(stack.top());
    stack.pop();
  }
  return result;
}

sedov::lli_t sedov::calculate(const Expression & postfix)
{
  const Queue< std::string > & postfixTokens = postfix.getTokens();
  Queue< std::string > postfixNew = postfixTokens;
  List< std::string > funcNames;
  funcNames.pushBack("+");
  funcNames.pushBack("-");
  funcNames.pushBack("*");
  funcNames.pushBack("/");
  funcNames.pushBack("%");
  funcNames.pushBack(">>");
  List< func_t > funcs;
  funcs.pushBack(add);
  funcs.pushBack(sub);
  funcs.pushBack(mult);
  funcs.pushBack(div);
  funcs.pushBack(mod);
  funcs.pushBack(bitRightShift);
  Stack< lli_t > nums;
  while (!postfixNew.empty())
  {
    std::string sym = postfixNew.front();
    postfixNew.pop();
    if (!isOperation(sym))
    {
      nums.push(std::stoll(sym));
    }
    else
    {
      if (nums.size() < 2)
      {
        throw std::invalid_argument("Input error: not enough operands");
      }

      sedov::func_t op = getFunction(sym, funcNames, funcs);
      if (!op)
      {
        throw std::invalid_argument("Input error: unknown operation");
      }

      lli_t right = nums.top();
      nums.pop();
      lli_t left = nums.top();
      nums.pop();
      lli_t newNum = op(left, right);
      nums.push(newNum);
    }
  }
  if (nums.size() != 1)
  {
    throw std::invalid_argument("Input error: invalid expression");
  }
  return nums.top();
}

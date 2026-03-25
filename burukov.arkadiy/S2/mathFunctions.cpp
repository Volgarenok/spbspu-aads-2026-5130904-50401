#include "mathFunctions.hpp"
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace burukov
{
  bool isOperand(const std::string &sym)
  {
    return sym != "+" && sym != "-" && sym != "*"
        && sym != "/" && sym != "%" && sym != "&"
        && sym != "(" && sym != ")";
  }

  int getPriority(const std::string &sym)
  {
    if (sym == "&")
    {
      return 0;
    }
    if (sym == "+" || sym == "-")
    {
      return 1;
    }
    if (sym == "*" || sym == "/" || sym == "%")
    {
      return 2;
    }
    return -1;
  }

  lli_t add(const lli_t lhs, const lli_t rhs)
  {
    const lli_t maxVal = std::numeric_limits< lli_t >::max();
    const lli_t minVal = std::numeric_limits< lli_t >::min();
    if ((rhs > 0) && (lhs > maxVal - rhs))
    {
      throw std::overflow_error("Add overflow");
    }
    if ((rhs < 0) && (lhs < minVal - rhs))
    {
      throw std::overflow_error("Add overflow");
    }
    return lhs + rhs;
  }

  lli_t sub(const lli_t lhs, const lli_t rhs)
  {
    const lli_t maxVal = std::numeric_limits< lli_t >::max();
    const lli_t minVal = std::numeric_limits< lli_t >::min();
    if ((rhs > 0) && (lhs < minVal + rhs))
    {
      throw std::overflow_error("Sub overflow");
    }
    if ((rhs < 0) && (lhs > maxVal + rhs))
    {
      throw std::overflow_error("Sub overflow");
    }
    return lhs - rhs;
  }

  lli_t mul(const lli_t lhs, const lli_t rhs)
  {
    const lli_t maxVal = std::numeric_limits< lli_t >::max();
    const lli_t minVal = std::numeric_limits< lli_t >::min();
    if (lhs == 0 || rhs == 0)
    {
      return 0;
    }
    if (lhs == -1)
    {
      if (rhs == minVal)
      {
        throw std::overflow_error("Mul overflow");
      }
      return -rhs;
    }
    if (rhs == -1)
    {
      if (lhs == minVal)
      {
        throw std::overflow_error("Mul overflow");
      }
      return -lhs;
    }
    if (lhs > 0)
    {
      if (rhs > 0)
      {
        if (lhs > maxVal / rhs)
        {
          throw std::overflow_error("Mul overflow");
        }
      }
      else
      {
        if (rhs < minVal / lhs)
        {
          throw std::overflow_error("Mul overflow");
        }
      }
    }
    else
    {
      if (rhs > 0)
      {
        if (lhs < minVal / rhs)
        {
          throw std::overflow_error("Mul overflow");
        }
      }
      else
      {
        if (lhs != 0 && rhs < maxVal / lhs)
        {
          throw std::overflow_error("Mul overflow");
        }
      }
    }
    return lhs * rhs;
  }

  lli_t div(const lli_t lhs, const lli_t rhs)
  {
    const lli_t minVal = std::numeric_limits< lli_t >::min();
    if (rhs == 0)
    {
      throw std::invalid_argument("Div by 0");
    }
    if ((lhs == minVal) && (rhs == -1))
    {
      throw std::overflow_error("Div overflow");
    }
    return lhs / rhs;
  }

  lli_t mod(const lli_t lhs, const lli_t rhs)
  {
    const lli_t minVal = std::numeric_limits< lli_t >::min();
    if (rhs == 0)
    {
      throw std::invalid_argument("Mod by 0");
    }
    if ((lhs == minVal) && (rhs == -1))
    {
      throw std::overflow_error("Mod overflow");
    }

    lli_t result = lhs % rhs;

    if ((result > 0) != (rhs > 0) && result != 0)
    {
        result += rhs;
    }
    return result;
  }

  lli_t bitwiseAnd(const lli_t lhs, const lli_t rhs)
  {
    return lhs & rhs;
  }

  void getInfix(std::istream &in,
      Stack< Queue< std::string > > &infix)
  {
    char sym = 0;
    std::string current;
    Queue< std::string > expression;
    while (in.get(sym))
    {
      if (sym == '\n')
      {
        if (!current.empty())
        {
          expression.push(current);
          current.clear();
        }
        if (!expression.empty())
        {
          infix.push(expression);
          expression.clear();
        }
      }
      else if (sym == ' ' || sym == '\t')
      {
        if (!current.empty())
        {
          expression.push(current);
          current.clear();
        }
      }
      else
      {
        current += sym;
      }
    }
    if (!current.empty())
    {
      expression.push(current);
    }
    if (!expression.empty())
    {
      infix.push(expression);
    }
  }

  void convertToPostfix(const Queue< std::string > &infix,
      Queue< std::string > &postfix)
  {
    Queue< std::string > infixCopy = infix;
    Stack< std::string > ops;
    while (!infixCopy.empty())
    {
      const std::string sym = infixCopy.front();
      infixCopy.pop();
      if (sym == "(")
      {
        ops.push(sym);
        continue;
      }
      if (sym == ")")
      {
        while (!ops.empty() && ops.top() != "(")
        {
          postfix.push(ops.top());
          ops.pop();
        }
        if (ops.empty())
        {
          throw std::invalid_argument("Mismatched parentheses");
        }
        ops.pop();
        continue;
      }
      if (isOperand(sym))
      {
        postfix.push(sym);
      }
      else
      {
        while (!ops.empty() && ops.top() != "("
            && getPriority(ops.top()) >= getPriority(sym))
        {
          postfix.push(ops.top());
          ops.pop();
        }
        ops.push(sym);
      }
    }
    while (!ops.empty())
    {
      if (ops.top() == "(")
      {
        throw std::invalid_argument("Mismatched parentheses");
      }
      postfix.push(ops.top());
      ops.pop();
    }
  }

  std::string calculate(const Queue< std::string > &postfix)
  {
    Queue< std::string > postfixCopy = postfix;
    List< std::string > funcNames;
    funcNames.pushFront("&");
    funcNames.pushFront("%");
    funcNames.pushFront("/");
    funcNames.pushFront("*");
    funcNames.pushFront("-");
    funcNames.pushFront("+");

    List< func_t > funcs;
    funcs.pushFront(bitwiseAnd);
    funcs.pushFront(mod);
    funcs.pushFront(div);
    funcs.pushFront(mul);
    funcs.pushFront(sub);
    funcs.pushFront(add);

    Stack< std::string > nums;
    while (!postfixCopy.empty())
    {
      const std::string sym = postfixCopy.front();
      postfixCopy.pop();
      if (isOperand(sym))
      {
        nums.push(sym);
      }
      else
      {
        if (nums.size() < 2)
        {
          throw std::invalid_argument("Invalid expression");
        }

        size_t index = 0;
        LIter< std::string > nameIt = funcNames.begin();
        while (nameIt != funcNames.end() && (*nameIt) != sym)
        {
          ++nameIt;
          ++index;
        }

        const char *str1 = nums.top().c_str();
        char *end1 = nullptr;
        errno = 0;
        const lli_t num1 = std::strtoll(str1, &end1, 10);
        if (end1 == str1 || *end1 != '\0' || errno == ERANGE)
        {
          throw std::invalid_argument("Input error");
        }
        nums.pop();

        const char *str2 = nums.top().c_str();
        char *end2 = nullptr;
        errno = 0;
        const lli_t num2 = std::strtoll(str2, &end2, 10);
        if (end2 == str2 || *end2 != '\0' || errno == ERANGE)
        {
          throw std::invalid_argument("Input error");
        }
        nums.pop();

        LIter< func_t > funcIt = funcs.begin();
        for (size_t i = 0; i < index; ++i)
        {
          ++funcIt;
        }
        const lli_t result = (*funcIt)(num2, num1);
        nums.push(std::to_string(result));
      }
    }
    if (nums.size() != 1)
    {
      throw std::invalid_argument("Input error");
    }
    return nums.top();
  }
}

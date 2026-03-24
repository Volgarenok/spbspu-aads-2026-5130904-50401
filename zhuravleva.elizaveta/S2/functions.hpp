#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdexcept>
#include <string>
#include <cctype>
#include "stack.hpp"
#include "queue.hpp"
#include "operations.hpp"
namespace zhuravleva
{
  bool isOperator(const std::string& op)
  {
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "!" || op == "%")
    {
      return true;
    }
    return false;
  }

  bool isNumber(const std::string& num)
  {
    if (num.empty())
    {
      return false;
    }
    size_t i = 0;
    if (num[0] == '-')
    {
      i = 1;
    }
    for (; i < num.size(); i++)
    {
      if(!isdigit(num[i]))
      {
        return false;
      }
    }
    return true;
  }

  int priority(const std::string& op)
  {
    if (op == "!")
    {
      return 3;
    }else if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    } else if (op == "+" || op == "-")
    {
      return 1;
    }
    return 0;
  }

  zhuravleva::Queue< std::string > infToPostfix(const std::string& line)
  {
    zhuravleva::Queue< std::string > out;
    zhuravleva::Stack< std::string > ops;
    std::string part;
  
    for (size_t i = 0; i <= line.size(); i++)
    {
      if (i == line.size() || line[i] == ' ')
      {
        if (part.empty())
        {
          continue;
        }
        if (isNumber(part))
        {
          out.push(part);
        }else if (part == "(")
        {
          ops.push(part);
        }
        else if (part == ")")
        {
          while(!ops.empty() && ops.top() != "(")
          {
            out.push(ops.top());
            ops.pop();
          }
          if (ops.empty())
          {
            throw std::runtime_error("empty operators list error");
          }
          ops.pop();
        }else if (isOperator(part))
        {
          while( !ops.empty() && isOperator(ops.top()) && priority(ops.top()) >= priority(part))
          {
            out.push(ops.top());
            ops.pop();
          }
          ops.push(part);
        }else
        {
          throw std::runtime_error("not right symbol error");
        }
        part.clear();
      }else
      {
        part += line[i];
      }
    }

    while(!ops.empty())
    {
      if (ops.top() == "(")
      {
        throw std::runtime_error("there's a '(' in the stack and no ')' -> error");
      }
      out.push(ops.top());
      ops.pop();
    }
    return out;
  }


  zhuravleva::value_t applyBinary(const std::string& op, int a, int b)
  {
    if (op == "+")
    {
      return zhuravleva::add(a, b);
    }
    if (op == "-")
    {
      return zhuravleva::sub(a, b);
    }
    if (op == "*")
    {
      return zhuravleva::mul(a, b);
    }

    if (op == "/")
    {
      return zhuravleva::div(a, b);
    }

    if (op == "%")
    {
      return zhuravleva::mod(a, b);
    }

    throw std::runtime_error("error");
  }


  zhuravleva::value_t calcPostfix(zhuravleva::Queue< std::string > line)
  {
    zhuravleva::Stack<zhuravleva::value_t> st;
    while (!line.empty())
    {
      std::string t = line.front();
      line.pop();
      if (isNumber(t))
      {
        st.push(std::stoll(t));
      }else if (isOperator(t))
      {
        if (t == "!")
        {
          if (st.empty())
          {
            throw std::runtime_error("stack is empty error");
          }
          auto a = st.top();
          st.pop();
          st.push(zhuravleva::bitNot(a));
        }else
        {
          if (st.empty())
          {
            throw std::runtime_error("stack is empty error");
          }
          auto b = st.top();
          st.pop();
          if (st.empty())
          {
            throw std::runtime_error("stack is empty error");
          }
          auto a = st.top();
           st.pop();
           st.push(applyBinary(t, a, b));
        }
      }else
      {
        throw std::runtime_error("thats not a number or an operation error");
      }
    }
    if (st.empty())
    {
      throw std::runtime_error("no result in the stack error");
    }
    auto res = st.top();
    st.pop();
    if (!st.empty())
    {
      throw std::runtime_error("we got result(so stack is empty) but there's smth in the stack error");
    }
    return res;
  }

}

#endif
#include "utils.hpp"
#include <limits>
#include <stdexcept>
#include <string>
#include "Stack.hpp"
#include "Queue.hpp"

size_t petrov::detail::prior(char s)
{
  if (s == '+' || s == '-')
  {
    return 2;
  }
  else if (s == '*' || s == '/' || s == '%')
  {
    return 3;
  }
  if (s == '&')
  {
    return 1;
  }
  return 0;
}

long long int petrov::detail::oper(long long int a, long long int b, char s)
{
  long long int mmax = std::numeric_limits<long long int>::max();
  long long int mmin = std::numeric_limits<long long int>::min();
  if (s == '+')
  {
    if ((b > 0) && (a > mmax - b))
    {
      throw std::logic_error("err\n");
    }
    if ((b < 0) && (a < mmin - b))
    {
      throw std::logic_error("err\n");
    }
    return a + b;
  }

  if (s == '-')
  {
    if ((b > 0) && (a < mmin + b))
    {
      throw std::logic_error("err\n");
    }
    if ((b < 0) && (a > mmax + b))
    {
      throw std::logic_error("err\n");
    }
    return a - b;
  }

  if (s == '*')
  {
    if (a > 0)
    {
      if (b > 0)
      {
        if (a > mmax / b)
        {
          throw std::logic_error("err\n");
        }
      }
      else
      {
        if (b < mmin/a)
        {
          throw std::logic_error("err\n");
        }
      }
    }
    else if (a < 0)
    {
      if (b > 0)
      {
        if (a < mmin/b)
        {
          throw std::logic_error("err\n");
        }
      }
      else
      {
        if (a != 0 && b != 0)
        {
          if (a < mmax/b)
          {
            throw std::logic_error("err\n");
          }
        }
      }
    }
    return a*b;
  }

  if (s == '/')
  {
    if (b == 0)
    {
      throw std::logic_error("err\n");
    }
    if (a == mmin && b == -1)
    {
      throw std::logic_error("err\n");
    }
    return a/b;
  }

  if (s == '%')
  {
    if (b == 0)
    {
      throw std::logic_error("err\n");
    }
    long long res = a%b;
    if (res < 0)
    {
      res += (b < 0 ? -b : b);
    }
    return res;
  }

  throw std::logic_error("err\n");
}

size_t petrov::detail::isOp(char s)
{
  return s == '+' || s == '-' || s == '*' || s == '%' || s == '/' || s == '&';
}

petrov::Stack<long long int> petrov::calcStream(std::istream& in)
{
  petrov::Stack<long long int> res;
  std::string s;
  while (std::getline(in, s))
  {
    if (!s.empty())
    {
      petrov::Queue<std::string> post;
      petrov::Stack<char> c;
      for (size_t i = 0; i < s.size(); ++i)
      {
        if (s[i] != ' ')
        {
          if (s[i] == '(')
          {
            c.push(s[i]);
          }
          else if (s[i] == ')')
          {
            while (!c.empty() && c.top() != '(')
            {
              std::string q = "";
              q += c.drop();
              post.push(q);
            }
            if (c.empty())
            {
              throw std::logic_error("err\n");
            }
            c.pop();
          }
          else if (petrov::detail::isOp(s[i]))
          {
            while (!c.empty() && petrov::detail::prior(c.top()) >= petrov::detail::prior(s[i]))
            {
              std::string q = "";
              q += c.drop();
              post.push(q);
            }
            c.push(s[i]);
          }
          else
          {
            std::string n = "";
            while (i < s.size() && s[i] != ' ' && s[i] != '(' && s[i] != ')' && !petrov::detail::isOp(s[i]))
            {
              n += s[i++];
            }
            post.push(n);
            --i;
          }
        }
      }
      while (!c.empty())
      {
        if (c.top() == '(')
        {
          throw std::logic_error("err\n");
        }
        std::string q = "";
        q += c.drop();
        post.push(q);
      }

      petrov::Stack<long long int> b;
      while (!post.empty())
      {
        std::string t = post.drop();
        if (t.size() == 1 && petrov::detail::isOp(t[0]))
        {
          if (b.size() < 2)
          {
            throw std::logic_error("err\n");
          }
          long long int b1 = b.drop();
          long long int a = b.drop();
          b.push(petrov::detail::oper(a, b1, t[0]));
        }
        else
        {
          b.push(std::stoll(t));
        }
      }
      if (b.size() != 1)
      {
        throw std::logic_error("err\n");
      }
      res.push(b.drop());
    }
  }
  return res;
}

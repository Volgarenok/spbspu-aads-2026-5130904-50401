
#include "func-stack-queue.hpp"

#include <iostream>
#include <string>
#include <iosfwd>

#include "func-math.hpp"
#include "../common/my_queue/queue.hpp"
#include "../common/my_stack/stack.hpp"

namespace zubarev
{
  namespace detail
  {
    std::string readLine(std::istream& in)
    {
      std::string line;
      std::getline(in, line);
      return line;
    }

    Queue< std::string > fromStrToQueue(const std::string& str)
    {
      Queue< std::string > q;
      std::string container = "";
      for (auto it = str.begin(); it != str.end(); ++it) {
        char ch = *it;
        if (std::isspace(static_cast< unsigned char >(ch))) {
          continue;
        }
        if (std::isdigit(ch)) {
          container += (ch);
        } else {
          if (!container.empty()) {
            q.push(container);
            container = "";
          }
          if (ch == '#') {
            container = "";
            if (it + 1 != str.end() && *(it + 1) == '#') {
              q.push("##");
              ++it;
            } else {
              it++;
            }
          } else {
            q.push(std::string(1, ch));
          }
        }
      }
      if (!container.empty()) {
        q.push(container);
      }
      return q;
    }
    bool isOperation(const std::string& str)
    {
      return str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == "##";
    }
    size_t getPriority(const std::string& oper)
    {

      if (oper == "##") {
        return 3;
      }

      if (oper == "*" || oper == "/" || oper == "%") {
        return 2;
      }

      if (oper == "+" || oper == "-") {
        return 1;
      }
      return 0;
    }

    Queue< std::string > fromInfixToPostfix(Queue< std::string >& infixQ)
    {
      Stack< std::string > stack;
      Queue< std::string > postfixQ;

      while (!infixQ.empty()) {

        std::string el = infixQ.top();
        infixQ.drop();

        if (el == "(") {
          stack.push(el);
        } else if (el == ")") {

          while (!stack.empty() && stack.top() != "(") {
            postfixQ.push(stack.top());
            stack.drop();
          }

          if (stack.empty()) {
            throw std::runtime_error("Unbalanced parentheses");
          }

          stack.drop();
        } else if (isOperation(el)) {

          while (!stack.empty() && getPriority(stack.top()) >= getPriority(el)) {
            postfixQ.push(stack.top());
            stack.drop();
          }
          stack.push(el);
        } else {
          postfixQ.push(el);
        }
      }

      while (!stack.empty()) {
        if (stack.top() == "(") {
          throw std::runtime_error("Unbalanced parentheses");
        }
        postfixQ.push(stack.top());
        stack.drop();
      }

      return postfixQ;
    }

  }

}
void zubarev::run(std::istream& in, std::ostream& out)
{
  Stack< long long > results;
  std::string expression = "";
  while (std::getline(in, expression)) {
    Queue< std::string > infixQ = detail::fromStrToQueue(expression);
    if (infixQ.empty()) {
      continue;
    }

    Queue< std::string > postfixQ = detail::fromInfixToPostfix(infixQ);
    results.push(eval(postfixQ));
  }

  if (results.empty()) {
    out << '\n';
    return;
  }
  out << results.top();
  results.drop();
  while (!results.empty()) {
    out << ' ' << results.top();
    results.drop();
  }

  out << '\n';
}

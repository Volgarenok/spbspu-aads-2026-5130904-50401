#include "func-stack-queue.hpp"

#include <string>

#include "queue.hpp"
#include "stack.hpp"

zubarev::Queue< std::string > zubarev::detail::fromStrToQueue(const std::string& str)
{
  Queue< std::string > q;
  std::string container = "";
  if (str.empty()) {
    return q;
  }
  for (auto it = str.begin(); it != str.end(); ++it) {
    char ch = *it;

    if (std::isspace(static_cast< unsigned char >(ch))) {
      q.push(container);
      container = "";
    } else {
      container += (ch);
    }
  }
  if (!container.empty()) {
    q.push(container);
  }
  return q;
}
bool zubarev::detail::isOperation(const std::string& str)
{
  return str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == "##";
}
size_t zubarev::detail::getPriority(const std::string& oper)
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

zubarev::Queue< std::string > zubarev::detail::fromInfixToPostfix(Queue< std::string >& infixQ)
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

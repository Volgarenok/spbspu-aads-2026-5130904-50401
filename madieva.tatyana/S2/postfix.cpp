#include "postfix.hpp"
#include <stdexcept>
#include <string>

#include "math.hpp"

namespace
{
  int getPriority(const std::string & op)
  {
    if (op == "+" || op == "-") {
      return 1;
    }
    if (op == "*" || op == "/" || op == "%" || op == "gcd") {
      return 2;
    }
    return 0;
  }

  void handleOperator(const std::string & s, madieva::Stack< std::string > & op,
    madieva::Queue< std::string > & post)
  {
    int prior = getPriority(s);

    while (!op.empty() && op.top() != "(" && getPriority(op.top()) >= prior) {
      post.push(op.top());
      op.pop();
    }
    op.push(s);
  }

}

madieva::Queue< std::string > madieva::postfix(const std::string line)
{
  madieva::Stack< std::string > op;
  madieva::Queue< std::string > post;
  for (size_t i = 0; i < line.length(); ++i) {
    if (line[i] == ' ') {
      continue;
    }

    size_t start = i;
    while (i < line.length() && line[i] != ' ') {
      ++i;
    }
    const std::string token = line.substr(start, i - start);
    if (token == "(") {
      op.push(token);
    } else if (token == ")") {
      if (op.empty()) {
        throw std::runtime_error("Mismatched parentheses");
      }
      std::string temp = op.top();
      while (temp != "(") {
        post.push(temp);
        op.pop();
        if (op.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        temp = op.top();
      }
      op.pop();
    } else if (isOperator(token)) {
      handleOperator(token, op, post);
    } else {
      post.push(token);
    }
  }
  while (!op.empty()) {
    post.push(op.top());
    op.pop();
  }
  return post;
}

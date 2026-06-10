#include <fstream>
#include <iostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"
#include "math.hpp"

namespace madieva
{
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

    void postfix(std::string line, madieva::Queue< std::string > & post)
    {
      madieva::Stack< std::string > op;
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
    }
  }
}

int main(int argc, char * argv[])
{
  namespace mad = madieva;
  std::ifstream file;
  std::istream * in = &std::cin;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "file not open\n";
      return 1;
    }
    in = &file;
  } else if (argc > 2) {
    std::cerr  << "Not right size arguments\n";
    return 2;
  }
  mad::Stack< long long > res;
  std::string line;
  while (std::getline(*in, line)) {
    if (!line.empty()) {
      try {
        mad::Queue< std::string > post;
        mad::postfix(line, post);
        evaluateExpression(post, res);
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
      }
    }
  }
  if (!res.empty()) {
    std::cout << res.top();
    res.pop();
  }
  while (!res.empty()) {
    std::cout << " " << res.top();
    res.pop();
  }
  std::cout << "\n";
}

#include "all.hpp"
#include "func-math.hpp"
#include <iostream>
#include <limits>
namespace zubarev
{
  using ll_int = long long int;
  void output(std::istream& in)
  {
    Stack< ll_int > results;

    while (in && !in.eof()) {
      bool error = false;
      std::string expression = input(in, error);

      if (error) {
        throw std::runtime_error("input: incorrect input");
      }

      if (!expression.empty()) {
        Queue< std::string > infixQ = fromStrToQueue(expression);
        Queue< std::string > postfixQ = fromInfixToPostfix(infixQ);
        results.push(eval(postfixQ));
      }
    }

    while (!results.empty()) {
      std::cout << results.top() << (results.size() > 1 ? " " : "");
      results.drop();
    }
    std::cout << '\n';
  }
  std::string input(std::istream& in, bool& error)
  {
    error = false;
    std::string line;

    if (!std::getline(in, line)) {
      if (in.eof()) {
        return "";
      }
      error = true;
      return "";
    }

    std::string result;
    char ch;
    for (size_t i = 0; i < line.size(); ++i) {
      ch = line[i];
      if (std::isspace(static_cast< unsigned char >(ch))) {
        continue;
      }

      if (std::isdigit(static_cast< unsigned char >(ch)) || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
          ch == '(' || ch == ')' || ch == '%' || ch == '#') {
        result += ch;
      } else {
        error = true;
        std::cerr << "input: incorrect input (invalid char '" << ch << "')\n";
        return "";
      }
    }

    return result;
  }

  Queue< std::string > fromStrToQueue(std::string& str)
  {
    Queue< std::string > infixQ;
    size_t num = 0;
    while (num < str.size()) {
      if (std::isdigit(str[num])) {
        std::string container = "";
        while (num < str.size() && std::isdigit(str[num])) {
          container += str[num];
          num++;
        }
        infixQ.push(container);
      } else if (str[num] == '#') {

        std::string container = "";
        if (num + 2 < str.size() && str[num + 1] == '#') {
          container += str[num];
          container += str[num];
          num += 2;
          infixQ.push(container);
        } else {
          num++;
        }

      } else if (str[num] == '+' || str[num] == '-' || str[num] == '*' || str[num] == '/' || str[num] == '(' ||
                 str[num] == ')' || str[num] == '%') {
        infixQ.push(std::string(1, str[num]));
        num++;
      } else {
        num++;
      }
    }
    return infixQ;
  }
  bool isdigit(const std::string& str)
  {
    if (str.empty()) {
      return false;
    }

    for (size_t i = 0; i < str.size(); ++i) {
      if (!std::isdigit(str[i])) {
        return false;
      }
    }

    return true;
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
      if (isdigit(el)) {
        postfixQ.push(el);
      } else {
        if (el != "(" && el != ")") {
          while (!stack.empty() && getPriority(stack.top()) >= getPriority(el)) {
            postfixQ.push(stack.top());
            stack.drop();
          }
          stack.push(el);
        } else {
          if (el == ")") {
            while (!stack.empty() && stack.top() != "(") {
              postfixQ.push(stack.top());
              stack.drop();
              if (stack.empty()) {
                throw std::runtime_error("Unbalanced parentheses: missing '('");
              }
            }
            stack.drop();
          } else {
            stack.push(el);
          }
        }
      }

      infixQ.drop();
    }

    while (!stack.empty()) {
      postfixQ.push(stack.top());
      stack.drop();
    }
    return postfixQ;
  }

}

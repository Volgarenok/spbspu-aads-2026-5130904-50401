#include "all.hpp"
#include <iostream>
namespace zubarev
{
  using ll_int = long long int;
  std::string input(std::istream& in, bool& error)
  {
    std::string str = "";
    char ch;
    while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\n') {
      in.get();
    }
    while (in.peek() != '\n' && in.peek() != EOF) {
      ch = in.get();
      if (ch != ' ' && ch != '\t') {
        if (!isalpha(ch)) {
          str += ch;
        } else {
          error = true;
          std::cerr << "input: incorrect input" << '\n';
          return "";
        }
      }
    }
    if (in.peek() == '\n') {
      in.get();
    }

    if (str == "") {
      error = true;
    } else {
      error = false;
    }
    return str;
  }

  Queue< std::string > fromStrToQueue(std::string str)
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
          container += 2 * str[num];
          num += 2;
          infixQ.push(container);
        } else {
          num++;
        }

      } else if (str[num] == '+' || str[num] == '-' || str[num] == '*' || str[num] == '/' || str[num] == '(' ||
                 str[num] == ')') {
        infixQ.push(std::string(1, str[num]));
        num++;
      }

      else {

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
  Queue< std::string > fromInfixToPostfix(Queue< std::string > infixQ)
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
            while (stack.top() != "(") {
              postfixQ.push(stack.top());
              stack.drop();
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

  ll_int summation(size_t oper1, size_t oper2)
  {
    return oper1 + oper2;
  }
  ll_int subtraction(size_t oper1, size_t oper2)
  {
    return oper1 - oper2;
  }
  ll_int multiplication(size_t oper1, size_t oper2)
  {
    return oper1 * oper2;
  }
  ll_int division(size_t oper1, size_t oper2)
  {
    return oper1 / oper2;
  }
  ll_int remainder(size_t oper1, size_t oper2)
  {
    return oper1 % oper2;
  }
  ll_int concatenation(size_t oper1, size_t oper2)
  {
    return oper1 + oper2;
  }
  size_t fromStrToNum(std::string str)
  {
    size_t num = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      num = num * 10 + (str[i] - '0');
    }
    return num;
  }
  ll_int evil(Queue< std::string > postfixQ)
  {
    const std::string symbols[] = {"+", "-", "*", "/", "%", "##"};

    ll_int (*functions[])(size_t,
                          size_t) = {summation, subtraction, multiplication, division, remainder, concatenation};
    Stack< ll_int > res;
    while (!postfixQ.empty()) {
      while (!postfixQ.empty() && isdigit(postfixQ.top())) {
        res.push(fromStrToNum(postfixQ.top()));
        postfixQ.drop();
      }
      ll_int a = res.top();
      res.drop();
      ll_int b = res.top();
      res.drop();
      std::string operation = postfixQ.top();
      postfixQ.drop();

      size_t index = 0;
      for (size_t i = 0; i < 6; ++i) {
        if (operation == symbols[i]) {
          index = i;
          break;
        }
      }
      res.push(functions[index](b, a));
    }

    return res.top();
  }
}

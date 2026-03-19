#include "all.hpp"
#include <iostream>
#include <limits>
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

    // if (str == "") {
    //   error = true;
    // } else {
    //   error = false;
    // }
    return str;
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
            while (stack.top() != "(") {
              postfixQ.push(stack.top());
              stack.drop();
              if (stack.empty()) {
                throw std::runtime_error("Unbalanced parentheses: missing '('");
                ;
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

  ll_int summation(ll_int oper1, ll_int oper2)
  {
    if (oper2 > 0 && oper1 > std::numeric_limits< ll_int >::max() - oper2) {
      throw std::overflow_error("Addition overflow: positive");
    }
    if (oper2 < 0 && oper1 < std::numeric_limits< ll_int >::min() - oper2) {
      throw std::overflow_error("Addition overflow: negative");
    }
    return oper1 + oper2;
  }
  ll_int subtraction(ll_int oper1, ll_int oper2)
  {
    if (oper1 > std::numeric_limits< ll_int >::min() + oper2) {
      return oper1 - oper2;
    } else {
      throw std::overflow_error("Subtraction overflow");
      ;
    }
  }
  ll_int multiplication(ll_int oper1, ll_int oper2)
  {
    if (oper1 == 0 || oper2 == 0) {
      return 0;
    }

    if (oper1 == std::numeric_limits< ll_int >::min() && oper2 == -1) {
      throw std::overflow_error("Multiplication overflow: MIN * -1");
    }
    if (oper2 == std::numeric_limits< ll_int >::min() && oper1 == -1) {
      throw std::overflow_error("Multiplication overflow: MIN * -1");
    }

    if (oper1 > 0) {
      if (oper2 > 0) {

        if (oper1 > std::numeric_limits< ll_int >::max() / oper2) {
          throw std::overflow_error("Multiplication overflow: (+)*(+)");
        }
      } else {

        if (oper2 < std::numeric_limits< ll_int >::min() / oper1) {
          throw std::overflow_error("Multiplication overflow: (+)*(-)");
        }
      }
    } else {
      if (oper2 > 0) {
        if (oper1 < std::numeric_limits< ll_int >::min() / oper2) {
          throw std::overflow_error("Multiplication overflow: (-)*(+)");
        }
      } else {
        if (oper1 != 0 && oper2 < std::numeric_limits< ll_int >::max() / oper1) {
          throw std::overflow_error("Multiplication overflow: (-)*(-)");
        }
      }
    }

    return oper1 * oper2;
  }
  ll_int division(ll_int oper1, ll_int oper2)
  {
    if (oper2 == 0) {
      throw std::runtime_error("Division by zero");
    }
    if (oper1 == std::numeric_limits< ll_int >::min() && oper2 == -1) {
      throw std::overflow_error("Division overflow: MIN / -1");
    }
    return oper1 / oper2;
  }
  ll_int remainder(ll_int oper1, ll_int oper2)
  {
    if (oper2 != 0) {
      return oper1 % oper2;
    } else {
      throw std::runtime_error("Modulo by zero");
      ;
    }
  }
  ll_int concatenation(ll_int oper1, ll_int oper2)
  {
    ll_int res;
    if (oper2 < 0) {
      oper2 = -1 * oper2;
    }
    res = multiplication(oper1, pow(10, ceil(log10(oper2))));

    res = summation(res, oper2);
    return res;
  }

  ll_int fromStrToNum(std::string str)
  {
    ll_int num = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      ll_int digit = (str[i] - '0');
      if (num < (std::numeric_limits< ll_int >::max() - digit) / 10) {
        num = num * 10 + digit;
      } else {
        throw std::overflow_error("Number parsing overflow");
        ;
      }
    }
    return num;
  }
  ll_int eval(Queue< std::string >& postfixQ)
  {
    const std::string symbols[] = {"+", "-", "*", "/", "%", "##"};

    ll_int (*functions[])(ll_int,
                          ll_int) = {summation, subtraction, multiplication, division, remainder, concatenation};
    Stack< ll_int > res;
    ll_int oper1, oper2;
    std::string operation;
    while (!postfixQ.empty()) {
      while (!postfixQ.empty() && isdigit(postfixQ.top())) {
        res.push(fromStrToNum(postfixQ.top()));
        postfixQ.drop();
      }
      if (!res.empty() && !postfixQ.empty()) {
        oper1 = res.top();
        res.drop();
        oper2 = res.top();
        res.drop();
        operation = postfixQ.top();
        postfixQ.drop();
      } else {
        throw std::runtime_error("Invalid expression: not enough operands");
      }

      size_t index = 0;
      for (size_t i = 0; i < 6; ++i) {
        if (operation == symbols[i]) {
          index = i;
          break;
        }
      }
      res.push(functions[index](oper2, oper1));
    }
    if (!res.empty()) {
      return res.top();
    } else {
      throw std::runtime_error("Invalid expression: not enough operands");
    }
  }
}

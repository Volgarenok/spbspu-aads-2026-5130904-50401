#include "all.hpp"
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
    if ((oper2 > 0 && oper1 < std::numeric_limits< ll_int >::min() + oper2) ||
        (oper2 < 0 && oper1 > std::numeric_limits< ll_int >::max() + oper2)) {
      throw std::overflow_error("Subtraction overflow");
    }
    return oper1 - oper2;
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
    if (oper2 == 0) {
      throw std::runtime_error("Modulo by zero");
    }
    ll_int res = oper1 % oper2;
    if (res < 0) {
      res += (oper2 > 0 ? oper2 : -oper2); // или std::abs(oper2)
    }
    return res;
  }
  ll_int concatenation(ll_int oper1, ll_int oper2)
  {
    ll_int res;
    if (oper2 < 0) {
      oper2 = -1 * oper2;
    }
    ll_int temp = oper2;
    ll_int factor = 1;

    while (temp > 0) {
      factor = multiplication(factor, 10);
      temp /= 10;
    }

    res = summation(multiplication(oper1, factor), oper2);
    return res;
  }

  ll_int fromStrToNum(std::string& str)
  {
    ll_int num = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      ll_int digit = (str[i] - '0');
      if (num <= (std::numeric_limits< ll_int >::max() - digit) / 10) {
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
    while (!postfixQ.empty()) {
      std::string el = postfixQ.top();
      postfixQ.drop();
      if (isdigit(el)) {
        res.push(fromStrToNum(el));
      } else {
        if (res.size() < 2) {
          throw std::runtime_error("Invalid expression: not enough operands");
        } else {
          oper1 = res.top();
          res.drop();
          oper2 = res.top();
          res.drop();

          int index = -1;
          for (size_t i = 0; i < 6; ++i) {
            if (el == symbols[i]) {
              index = i;
              break;
            }
          }

          if (index != -1) {

            res.push(functions[index](oper2, oper1));
          } else {
            throw std::runtime_error("Invalid expression: incorrect operation");
          }
        }
      }
    }

    if (res.size() != 1) {
      throw std::runtime_error("Invalid expression: final stack size is not 1");
    }
    return res.top();
  }
}

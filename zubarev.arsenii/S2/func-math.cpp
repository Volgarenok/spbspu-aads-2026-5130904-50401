#include "all.hpp"
#include <iostream>
#include <limits>
namespace zubarev
{
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
      res += (oper2 > 0 ? oper2 : -oper2);  
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

#include "func-math.hpp"

#include <limits>
#include <string>

#include "../common/my_stack/stack.hpp"
long long zubarev::summation(long long oper1, long long oper2)
{
  if (oper2 > 0 && oper1 > std::numeric_limits< long long >::max() - oper2) {
    throw std::overflow_error("Addition overflow: positive");
  }
  if (oper2 < 0 && oper1 < std::numeric_limits< long long >::min() - oper2) {
    throw std::overflow_error("Addition overflow: negative");
  }
  return oper1 + oper2;
}
long long zubarev::subtraction(long long oper1, long long oper2)
{
  if ((oper2 > 0 && oper1 < std::numeric_limits< long long >::min() + oper2) ||
      (oper2 < 0 && oper1 > std::numeric_limits< long long >::max() + oper2)) {
    throw std::overflow_error("Subtraction overflow");
  }
  return oper1 - oper2;
}
long long zubarev::multiplication(long long oper1, long long oper2)
{
  if (oper1 == 0 || oper2 == 0) {
    return 0;
  }

  if (oper1 == std::numeric_limits< long long >::min() && oper2 == -1) {
    throw std::overflow_error("Multiplication overflow: MIN * -1");
  }
  if (oper2 == std::numeric_limits< long long >::min() && oper1 == -1) {
    throw std::overflow_error("Multiplication overflow: MIN * -1");
  }

  if (oper1 > 0) {
    if (oper2 > 0) {

      if (oper1 > std::numeric_limits< long long >::max() / oper2) {
        throw std::overflow_error("Multiplication overflow: (+)*(+)");
      }
    } else {

      if (oper2 < std::numeric_limits< long long >::min() / oper1) {
        throw std::overflow_error("Multiplication overflow: (+)*(-)");
      }
    }
  } else {
    if (oper2 > 0) {
      if (oper1 < std::numeric_limits< long long >::min() / oper2) {
        throw std::overflow_error("Multiplication overflow: (-)*(+)");
      }
    } else {
      if (oper1 != 0 && oper2 < std::numeric_limits< long long >::max() / oper1) {
        throw std::overflow_error("Multiplication overflow: (-)*(-)");
      }
    }
  }

  return oper1 * oper2;
}
long long zubarev::division(long long oper1, long long oper2)
{
  if (oper2 == 0) {
    throw std::runtime_error("Division by zero");
  }
  if (oper1 == std::numeric_limits< long long >::min() && oper2 == -1) {
    throw std::overflow_error("Division overflow: MIN / -1");
  }
  return oper1 / oper2;
}
long long zubarev::remainder(long long oper1, long long oper2)
{
  if (oper2 == 0) {
    throw std::runtime_error("Modulo by zero");
  }
  long long res = oper1 % oper2;
  if (res < 0) {
    res += (oper2 > 0 ? oper2 : -oper2);
  }
  return res;
}
long long zubarev::concatenation(long long oper1, long long oper2)
{
  long long res;
  if (oper2 < 0) {
    oper2 = -1 * oper2;
  }
  long long temp = oper2;
  long long factor = 1;

  while (temp > 0) {
    factor = multiplication(factor, 10);
    temp /= 10;
  }

  res = summation(multiplication(oper1, factor), oper2);
  return res;
}

long long (*zubarev::getOperation(const std::string& op))(long long, long long)
{
  if (op == "+") {
    return summation;
  }
  if (op == "-") {
    return subtraction;
  }
  if (op == "*") {
    return multiplication;
  }
  if (op == "/") {
    return division;
  }
  if (op == "%") {
    return remainder;
  }
  if (op == "##") {
    return concatenation;
  }

  return nullptr;
}
long long zubarev::eval(Queue< std::string >& postfixQ)
{
  Stack< long long > res;

  while (!postfixQ.empty()) {
    std::string el = postfixQ.top();
    postfixQ.drop();

    try {
      res.push(std::stoll(el));
    } catch (const std::exception&) {

      auto operation = getOperation(el);

      if (!operation) {
        throw std::runtime_error("Invalid operation");
      }

      if (res.size() < 2) {
        throw std::runtime_error("Not enough operands");
      }

      long long oper1 = res.top();
      res.drop();

      long long oper2 = res.top();
      res.drop();

      res.push(operation(oper2, oper1));
    }
  }

  if (res.size() != 1) {
    throw std::runtime_error("Invalid expression");
  }

  return res.top();
}

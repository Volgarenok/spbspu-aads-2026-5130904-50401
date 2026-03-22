#include "math_funcs.hpp"
#include <limits>
#include <climits>

const kuznetsov::lli_t MAX = std::numeric_limits< kuznetsov::lli_t >::max();
const kuznetsov::lli_t MIN = std::numeric_limits< kuznetsov::lli_t >::min();

bool kuznetsov::isOperand(const std::string& c)
{
  std::string operators[] = {"+", "-", "*", "/", "%", ">>", "(", ")"};
  for (size_t i = 0; i < 8; ++i) {
    if (c == operators[i]) {
      return false;
    }
  }
  return true;
}

size_t kuznetsov::getPriority(const std::string& c)
{
  if (c == ">>") {
    return 0;
  } else if (c == "+" || c == "-") {
    return 1;
  } else {
    return 2;
  }
}

kuznetsov::lli_t kuznetsov::add(const lli_t& a, const lli_t& b)
{
  if (a > 0 && b > 0 && a > MAX - b) {
    throw std::overflow_error("Add overflow");
  } else if (a < 0 && b < 0 && a < MIN - b) {
    throw std::overflow_error("Add overflow");
  }
  return a + b;
}

kuznetsov::lli_t kuznetsov::sub(const lli_t& a, const lli_t& b)
{
  if (a < 0 && b > 0 && a < MIN + b) {
    throw std::overflow_error("Sub overflow");
  } else if (a > 0 && b < 0 && a < MAX + b) {
    throw std::overflow_error("Sub overflow");
  }
  return a - b;
}

kuznetsov::lli_t kuznetsov::mul(const lli_t& a, const lli_t& b)
{
  if (!a || !b) {
    return 0;
  }
  if (a > 0 && b > 0 && a > MAX / b) {
    throw std::overflow_error("Multiply overflow");
  } else if (a < 0 && b < 0 && a > MAX / b) {
    throw std::overflow_error("Multiply overflow");
  } else if (a > 0 && b < 0 && b < MIN / a) {
    throw std::overflow_error("Multiply overflow");
  } else if (a < 0 && b > 0 && a < MIN / b) {
    throw std::overflow_error("Multiply overflow");
  }
  return a * b;
}

kuznetsov::lli_t kuznetsov::div(const lli_t& a, const lli_t& b)
{
  if (b == 0) {
    throw std::logic_error("Dont div by zero");
  }
  if (a == MIN && b == -1) {
    throw std::overflow_error("Div overflow");
  }
  return a / b;
}

kuznetsov::lli_t kuznetsov::mod(const lli_t& a, const lli_t& b)
{
  if (b == 0) {
    throw std::logic_error("Dont div by zero");
  }
  return a % b;
}

kuznetsov::lli_t kuznetsov::bitShiftToRight(const lli_t& a, const lli_t& b)
{
  if (b == 0) {
    throw std::logic_error("Bit right shift by value less then zero");
  }
  const int totalBits = sizeof(lli_t) * CHAR_BIT;
  if (b > totalBits || a == 0) {
    return 0;
  }
  return a >> b;
}

void kuznetsov::getExpressions(std::istream& in, stackOfinfixExpression& res)
{
  std::string current;
  Queue< std::string > expression;
  int a = in.get();
  while (a != -1) {
    if (a == '\n') {
      if (!current.empty()) {
        expression.push(current);
        current.clear();
      }
      if (!expression.empty()) {
        res.push(expression);
        expression.clear();
      }
    } else if (a == ' ') {
      expression.push(current);
      current.clear();
    } else {
      current.push_back(static_cast< char >(a));
    }
    a = in.get();
  }
  if (!current.empty()) {
    expression.push(current);
    current.clear();
  }
  if (!expression.empty()) {
    res.push(expression);
    expression.clear();
  }
}


kuznetsov::lli_t kuznetsov::calculate(Queue<std::string> postfix)
{
  Stack<lli_t> evalStack;
  while (!postfix.empty()) {
    std::string sym = postfix.front();
    postfix.pop();

    if (isOperand(sym)) {
      evalStack.push(std::stoll(sym));
    } else {
      if (evalStack.size() < 2) {
        throw std::logic_error("Not enough operands for operator: " + sym);
      }
      lli_t b = evalStack.top();
      evalStack.pop();
      lli_t a = evalStack.top();
      evalStack.pop();

      lli_t result = 0;
      if (sym == "+") {
        result = add(a, b);
      } else if (sym == "-") {
        result = sub(a, b);
      } else if (sym == "*") {
        result = mul(a, b);
      } else if (sym == "/") {
        result = div(a, b);
      } else if (sym == "%") {
        result = mod(a, b);
      } else if (sym == ">>") {
        result = bitShiftToRight(a, b);
      } else {
        throw std::logic_error("Unknown operator: " + sym);
      }

      evalStack.push(result);
    }
  }

  if (evalStack.size() != 1) {
    throw std::logic_error("Must be one res ");
  }

  return evalStack.top();
}

void kuznetsov::calculate(stackOfinfixExpression infix, Queue<lli_t>& res)
{
  Queue<std::string> postfix;
  Stack<std::string> temp;
  while (!infix.empty()) {
    Queue<std::string> curr = infix.top();
    infix.pop();
    while (!curr.empty()) {
      std::string sym = curr.front();
      curr.pop();
      if (sym == "(") {
        temp.push(sym);
      } else if (sym == ")") {
        while (!temp.empty() && temp.top() != "(") {
          postfix.push(temp.top());
          temp.pop();
        }
        temp.pop();
      } else if (isOperand(sym)) {
        postfix.push(sym);
      } else {
        while (!temp.empty() && temp.top() != "(") {
          if (getPriority(sym) <= getPriority(temp.top())) {
            postfix.push(temp.top());
            temp.pop();
          } else {
            break;
          }
        }
        temp.push(sym);
      }
    }
    while (!temp.empty()) {
      postfix.push(temp.top());
      temp.pop();
    }
    temp.clear();
    res.push(calculate(postfix));
    postfix.clear();
  }
}

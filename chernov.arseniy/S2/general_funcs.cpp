#include "general_funcs.hpp"

#include <cctype>
#include <stdexcept>

#include "math_funcs.hpp"

chernov::Queue< std::string > chernov::processLine(const std::string & line)
{
  Queue< std::string > math_expression;
  size_t i = 0;
  while (i < line.size()) {
    size_t j = i;
    while (j < line.size() && !std::isspace(line[j])) {
      ++j;
    }
    if (i != j) {
      math_expression.push(line.substr(i, j - i));
    }
    i = j + 1;
  }
  return math_expression;
}

void chernov::executeOperation(Stack< long long > & result, const std::string & oper)
{
  if (result.size() < 2) {
    throw std::runtime_error("invalid math expression: few operands");
  }

  long long b = result.top();
  result.pop();
  long long a = result.top();
  result.pop();

  long long c = 0;
  if (oper == "lcm") {
    c = lcm(a, b);
  } else if (oper == "*") {
    c = mul(a, b);
  } else if (oper == "/") {
    c = div(a, b);
  } else if (oper == "%") {
    c = mod(a, b);
  } else if (oper == "+") {
    c = add(a, b);
  } else if (oper == "-") {
    c = sub(a, b);
  } else {
    throw std::runtime_error("invalid math expression: unknown operator (" + oper + ")");
  }

  result.push(c);
}

long long chernov::calculateMathExpression(Queue< std::string > math_expression)
{
  Stack< long long > result;
  Stack< std::string > stack;

  while (!math_expression.empty()) {
    std::string element = math_expression.front();
    math_expression.pop();

    if (element == "(") {
      stack.push(element);
    } else if (element == ")") {
      while (!stack.empty() && stack.top() != "(") {
        executeOperation(result, stack.top());
        stack.pop();
      }
      if (stack.empty()) {
        throw std::runtime_error("invalid math expression: opening bracket is missing");
      }
      stack.pop();
    } else if (isOperand(element)) {
      result.push(std::stoll(element));
    } else if (!isOperator(element)) {
      throw std::runtime_error("invalid math expression: element is not operator or operand");
    } else {
      while (!stack.empty() && stack.top() != "(" && (getPriority(element) <= getPriority(stack.top()))) {
        executeOperation(result, stack.top());
        stack.pop();
      }
      stack.push(element);
    }
  }

  while (!stack.empty() && stack.top() != "(") {
    executeOperation(result, stack.top());
    stack.pop();
  }

  if (!stack.empty()) {
    throw std::runtime_error("invalid math expression: extra opening bracket");
  }
  if (result.size() != 1) {
    throw std::runtime_error("invalid math expression: few operators");
  }

  return result.top();
}

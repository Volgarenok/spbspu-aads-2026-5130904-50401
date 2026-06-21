#include "math_funcs.hpp"

#include <string>
#include <stdexcept>
#include <limits>

size_t lukashevich::detail::getOperatorType(const std::string& token)
{
  if (token == "!") {
    return 2;
  }
  if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%") {
    return 1;
  }
  return 0;
}

bool  lukashevich::detail::isOperator(const std::string& token)
{
  return getOperatorType(token) != 0;
}

size_t lukashevich::detail::getPriority(const std::string& token)
{
  if (token == "!") {
    return 3;
  }
  if (token == "*" || token == "/" || token == "%") {
    return 2;
  }
  if (token == "+" || token == "-") {
    return 1;
  }
  return 0;
}

lukashevich::List< std::string > lukashevich::tokenize(const std::string& str)
{
  List< std::string > res;
  std::string cur_token;

  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] != ' ') {
      cur_token += str[i];
    } else if (!cur_token.empty()) {
      res.pushBack(cur_token);
      cur_token.clear();
    }
  }

  if (!cur_token.empty()) {
    res.pushBack(cur_token);
  }

  return res;
}

lukashevich::Queue< std::string > lukashevich::infixToPostfix(const List< std::string >& tokens)
{
  Queue< std::string > output;
  Stack< std::string > op_stack;

  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    const std::string& token = *it;

    if (token == "(") {
      op_stack.push(token);
    } else if (token == ")") {
      while (!op_stack.empty() && op_stack.top() != "(") {
        output.push(op_stack.top());
        op_stack.pop();
      }

      if (op_stack.empty()) {
        throw std::runtime_error("invalid brackets");
      }

      op_stack.pop();
    } else if (detail::isOperator(token)) {
      while (!op_stack.empty() && op_stack.top() != "(") {
        const std::size_t topPriority = detail::getPriority(op_stack.top());
        const std::size_t currentPriority = detail::getPriority(token);
        const bool hasHigherPriority = topPriority > currentPriority;
        const bool hasEqualLeftPriority = topPriority == currentPriority && token != "!";

        if (hasHigherPriority || hasEqualLeftPriority) {
          output.push(op_stack.top());
          op_stack.pop();
        } else {
          break;
        }
      }

      op_stack.push(token);
    } else {
      output.push(token);
    }
  }

  while (!op_stack.empty()) {
    if (op_stack.top() == "(") {
      throw std::runtime_error("invalid brackets");
    }
    output.push(op_stack.top());
    op_stack.pop();
  }

  return output;
}

lukashevich::ll lukashevich::add(lukashevich::ll lhs, lukashevich::ll rhs)
{
  const bool upperOverflow = rhs > 0 && lhs > std::numeric_limits< ll >::max() - rhs;
  const bool lowerOverflow = rhs < 0 && lhs < std::numeric_limits< ll >::min() - rhs;

  if (upperOverflow || lowerOverflow) {
    throw std::overflow_error("overflow");
  }
  return lhs + rhs;
}

lukashevich::ll lukashevich::sub(lukashevich::ll lhs, lukashevich::ll rhs)
{
  const bool upperOverflow = rhs < 0 && lhs > std::numeric_limits< ll >::max() + rhs;
  const bool lowerOverflow = rhs > 0 && lhs < std::numeric_limits< ll >::min() + rhs;

  if (upperOverflow || lowerOverflow) {
    throw std::overflow_error("overflow");
  }
  return lhs - rhs;
}

lukashevich::ll lukashevich::mul(lukashevich::ll lhs, lukashevich::ll rhs)
{
  if (lhs == 0 || rhs == 0) {
    return 0;
  }
  if (lhs == -1 && rhs == std::numeric_limits< ll >::min()) {
    throw std::overflow_error("overflow");
  }
  if (rhs == -1 && lhs == std::numeric_limits< ll >::min()) {
    throw std::overflow_error("overflow");
  }
  if (lhs > 0) {
    if (rhs > 0) {
      if (lhs > std::numeric_limits< ll >::max() / rhs) {
        throw std::overflow_error("overflow");
      }
    }
    else {
      if (rhs < std::numeric_limits< ll >::min() / lhs) {
        throw std::overflow_error("overflow");
      }
    }
  }
  else {
    if (rhs > 0) {
      if (lhs < std::numeric_limits< ll >::min() / rhs) {
        throw std::overflow_error("overflow");
      }
    }
    else {
      if (rhs < std::numeric_limits< ll >::max() / lhs) {
        throw std::overflow_error("overflow");
      }
    }
  }
  return lhs * rhs;
}

lukashevich::ll lukashevich::divide(lukashevich::ll lhs, lukashevich::ll rhs)
{
  if (rhs == 0) {
    throw std::runtime_error("division by zero");
  }
  if (lhs == std::numeric_limits< ll >::min() && rhs == -1) {
    throw std::overflow_error("overflow");
  }
  return lhs / rhs;
}

lukashevich::ll lukashevich::mod(lukashevich::ll lhs, lukashevich::ll rhs)
{
  if (rhs == 0) {
    throw std::runtime_error("modulo by zero");
  }

  if (lhs == std::numeric_limits< ll >::min() && rhs == -1) {
    throw std::overflow_error("overflow");
  }

  ll result = lhs % rhs;

  if (result < 0) {
    result += (rhs > 0 ? rhs : -rhs);
  }
  return result;
}

lukashevich::ll lukashevich::bitwise_not(lukashevich::ll value)
{
  return ~value;
}

lukashevich::ll lukashevich::parse_ll(const std::string& s)
{
  std::size_t position = 0;

  try {
    const ll result = std::stoll(s, &position);

    if (position != s.size()) {
      throw std::runtime_error("invalid number format");
    }

    return result;
  }
  catch (const std::invalid_argument&) {
    throw std::runtime_error("invalid number format");
  }
  catch (const std::out_of_range&) {
    throw std::overflow_error("overflow");
  }
}

lukashevich::ll lukashevich::evaluate_expression(const std::string& line)
{
  List< std::string > tokens = tokenize(line);
  Queue< std::string > postfix = infixToPostfix(tokens);
  Stack< ll > eval_stack;

   while (!postfix.empty()) {
    const std::string token = postfix.front();
    postfix.pop();

    const std::size_t type = detail::getOperatorType(token);

    if (type == 0) {
      eval_stack.push(parse_ll(token));
    } else if (type == 1) {
      if (eval_stack.size() < 2) {
        throw std::runtime_error("invalid expression: missing operand");
      }

      const ll rhs = eval_stack.top();
      eval_stack.pop();

      const ll lhs = eval_stack.top();
      eval_stack.pop();

      ll result = 0;

      if (token == "+") {
        result = add(lhs, rhs);
      } else if (token == "-") {
        result = sub(lhs, rhs);
      } else if (token == "*") {
        result = mul(lhs, rhs);
      } else if (token == "/") {
        result = divide(lhs, rhs);
      } else {
        result = mod(lhs, rhs);
      }

      eval_stack.push(result);
    } else if (type == 2) {
      if (eval_stack.empty()) {
        throw std::runtime_error("invalid expression: missing operand for !");
      }

      const ll value = eval_stack.top();
      eval_stack.pop();

      eval_stack.push(bitwise_not(value));
    }
  }

  if (eval_stack.size() != 1) {
    throw std::runtime_error("invalid expression");
  }

  const ll result = eval_stack.top();
  eval_stack.pop();

  return result;
}


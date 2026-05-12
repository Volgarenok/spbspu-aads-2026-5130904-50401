#include "math_funcs.hpp"
#include <string>
#include <stdexcept>
#include <limits>

namespace lukashevich
{
  size_t getOperatorType(const std::string& token)
  {
    if (token == "!") {
      return 2;
    }
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%") {
      return 1;
    }
    return 0;
  }

  bool isOperator(const std::string& token)
  {
    return getOperatorType(token) != 0;
  }

  size_t getPriority(const std::string& token)
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

  List< std::string > tokenize(const std::string& str)
  {
    List< std::string > res;
    std::string cur_token = "";

    for (size_t i = 0; i < str.size(); ++i) {
      if (str[i] != ' ') {
        cur_token += str[i];
      }
      else if (!cur_token.empty()) {
        res.pushBack(cur_token);
        cur_token = "";
      }
    }
    if (!cur_token.empty()) {
      res.pushBack(cur_token);
    }
    return res;
  }

  List< std::string > infixToPostfix(const List< std::string >& tokens)
  {
    List< std::string > output;
    Stack< std::string > op_stack;

    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
      const std::string& token = *it;
      if (token == "(") {
        op_stack.push(token);
      }

      else if (token == ")") {
        while (!op_stack.empty() && op_stack.first() != "(") {
          output.pushBack(op_stack.drop());
        }
        if (op_stack.empty()) {
          throw std::runtime_error("invalid brackets");
        }
        op_stack.drop();
      }

      else if (isOperator(token)) {
        while (!op_stack.empty() && op_stack.first() != "(") {
          size_t top_prec = getPriority(op_stack.first());
          size_t cur_prec = getPriority(token);
          if (top_prec > cur_prec || (top_prec == cur_prec && token != "!")) {
            output.pushBack(op_stack.drop());
          }
          else {
            break;
          }
        }
        op_stack.push(token);
      }
      else {
        output.pushBack(token);
      }
    }

    while (!op_stack.empty()) {
      if (op_stack.first() == "(") {
        throw std::runtime_error("invalid brackets");
      }
      output.pushBack(op_stack.drop());
    }
    return output;
  }

  ll add(ll lhs, ll rhs)
  {
    if ((rhs > 0 && lhs > std::numeric_limits< ll >::max() - rhs) || (rhs < 0 && lhs < std::numeric_limits< ll >::min() - rhs)) {
      throw std::overflow_error("overflow");
    }
    return lhs + rhs;
  }

  ll sub(ll lhs, ll rhs)
  {
    if ((rhs < 0 && lhs > std::numeric_limits< ll >::max() + rhs) || (rhs > 0 && lhs < std::numeric_limits< ll >::min() + rhs)) {
      throw std::overflow_error("overflow");
    }
    return lhs - rhs;
  }

  ll mul(ll lhs, ll rhs)
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

  ll divide(ll lhs, ll rhs)
  {
    if (rhs == 0) {
      throw std::runtime_error("division by zero");
    }
    if (lhs == std::numeric_limits< ll >::min() && rhs == -1) {
      throw std::overflow_error("overflow");
    }
    return lhs / rhs;
  }

  ll mod(ll lhs, ll rhs)
  {
    if (rhs == 0) {
      throw std::runtime_error("modulo by zero");
    }
    if (lhs == std::numeric_limits< ll >::min() && rhs == -1) {
      throw std::overflow_error("overflow");
    }
    return lhs % rhs;
  }

  ll bitwise_not(ll value)
  {
    return ~value;
  }

  ll parse_ll(const std::string& s)
  {
    ll res = 0;
    size_t i = 0;
    bool neg = false;
    
    if (i < s.length() && s[i] == '-') {
      neg = true;
      ++i;
    }
    for (; i < s.length(); ++i) {
      if (s[i] < '0' || s[i] > '9') {
        throw std::runtime_error("invalid number format");
      }
      ll digit = s[i] - '0';
      const ll div_limit = std::numeric_limits< ll >::max() / 10;
      const ll mod_limit = std::numeric_limits< ll >::max() % 10;
      const ll allowed_digit = neg ? (mod_limit + 1) : mod_limit;

      if (res > div_limit || (res == div_limit && digit > allowed_digit)) {
        throw std::overflow_error("overflow");
      }
      res = res * 10 + digit;
    }
    return neg ? -res : res;
  }
}

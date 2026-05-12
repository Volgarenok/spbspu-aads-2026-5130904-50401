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
}

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
}

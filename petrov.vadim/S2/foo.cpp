#include "foo.hpp"

namespace petrov
{
  bool isOperator(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "%" || token == "#";
  }
  
  size_t getPriority(const std::string& op)
  {
    if (op == "#") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
  }
}

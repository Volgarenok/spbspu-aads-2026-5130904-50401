#include "math_functions.hpp"

namespace ulanova
{
  bool is_operator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
  }
  
}
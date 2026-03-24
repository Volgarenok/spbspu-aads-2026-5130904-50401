#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTION_HPP
#include <stdexcept>

namespace ulanova
{
  bool is_operator(char c);
  int priority(char op);
  long long apply_op(long long a, long long b, char op);
}
#endif

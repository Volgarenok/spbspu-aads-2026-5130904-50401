#ifndef MATH_HPP
#define MATH_HPP
#include <limits>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace madieva {

  const long long MAX = std::numeric_limits<long long>::max();
  const long long MIN = std::numeric_limits<long long>::min();

  bool isOperator(const std::string& token);
  long long computeGcd(long long a, long long b);
  long long add(long long left, long long right);
  long long subtract(long long left, long long right);
  long long multiply(long long left, long long right);
  long long divide(long long left, long long right);
  long long modulo(long long left, long long right);

  long long calculateOperation(const std::string & op,  long long left, long long right);
  void evaluateExpression(madieva::Queue< std::string > & post, madieva::Stack< long long > & res);

}

#endif

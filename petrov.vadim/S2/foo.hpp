#ifndef FOO_HPP
#define FOO_HPP

#include <string>
#include <cstddef>
#include <istream>
#include "stack.hpp"
#include "queue.hpp"
#include "math.hpp"

namespace petrov
{
  bool isOperator(const std::string& token);
  size_t getPriority(const std::string& op);
  bool isNumber(const std::string& token);
  void getInfix(const std::istream& in, Stack< Queue < std::string > >& data);
  void infixToPostfix(Stack< Queue < std::string > >& data, Stack< Queue < std::string > >& res);
  ll strToNum(const std::string& str);
  ll pickOperation(ll a, ll b, const std::string& op);
  ll pickReverse(ll a, ll b, const std::string& op);
}

#endif

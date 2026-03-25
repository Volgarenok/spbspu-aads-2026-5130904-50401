#include "mathFunctions.hpp"
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>

bool burukov::isOperand(const std::string &sym)
{
  return sym != "+" && sym != "-" && sym != "*"
      && sym != "/" && sym != "%" && sym != "&"
      && sym != "(" && sym != ")";
}

int burukov::getPriority(const std::string &sym)
{
  if (sym == "&")
  {
    return 0;
  }
  if (sym == "+" || sym == "-")
  {
    return 1;
  }
  if (sym == "*" || sym == "/" || sym == "%")
  {
    return 2;
  }
  return -1;
}

burukov::lli_t burukov::add(const lli_t, const lli_t)
{
  return 0;
}

burukov::lli_t burukov::sub(const lli_t, const lli_t)
{
  return 0;
}

burukov::lli_t burukov::mul(const lli_t, const lli_t)
{
  return 0;
}

burukov::lli_t burukov::div(const lli_t, const lli_t)
{
  return 0;
}

burukov::lli_t burukov::mod(const lli_t, const lli_t)
{
  return 0;
}

burukov::lli_t burukov::bitwiseAnd(const lli_t, const lli_t)
{
  return 0;
}

void burukov::getInfix(std::istream &,
    Stack< Queue< std::string > > &)
{
}

void burukov::convertToPostfix(const Queue< std::string > &,
    Queue< std::string > &)
{
}

std::string burukov::calculate(const Queue< std::string > &)
{
  return "0";
}

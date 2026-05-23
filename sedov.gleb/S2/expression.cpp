#include "expression.hpp"

void sedov::Expression::addToken(const std::string & token)
{
  tokens_.push(token);
}

bool sedov::Expression::empty() const
{
  return tokens_.empty();
}

size_t sedov::Expression::size() const
{
  return tokens_.size();
}

void sedov::Expression::clear()
{
  tokens_.clear();
}

const sedov::Queue< std::string > & sedov::Expression::getTokens() const
{
  return tokens_;
}

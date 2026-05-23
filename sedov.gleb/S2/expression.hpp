#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <string>
#include "queue.hpp"

namespace sedov
{
  class Expression
  {
  public:
    void addToken(const std::string & token);
    bool empty() const;
    size_t size() const;
    void clear();
    const Queue< std::string > & getTokens() const;

  private:
    Queue< std::string > tokens_;
  };
}

#endif

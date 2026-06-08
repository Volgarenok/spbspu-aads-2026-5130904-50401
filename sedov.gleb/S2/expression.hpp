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
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    const Queue< std::string > & getTokens() const noexcept;

  private:
    Queue< std::string > tokens_;
  };
}

#endif

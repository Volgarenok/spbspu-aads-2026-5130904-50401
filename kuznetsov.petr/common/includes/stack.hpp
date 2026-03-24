#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace kuznetsov {
  template< class T >
  class Stack {
  public:

    T& top()
    {
      return list_.front();
    }

    const T& top() const
    {
      return list_.front();
    }

    bool empty() const

    {
      return list_.empty();
    }
    size_t size() const
    {
      return list_.size();
    }

    void push(const T& val)
    {
      list_.insert(list_.begin(), val);
    }
    void pop()
    {
      list_.popFront();
    }

    void clear()
    {
      list_.clear();
    }

  private:
    List< T > list_;
  };


}
#endif

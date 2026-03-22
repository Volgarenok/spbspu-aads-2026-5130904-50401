#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace kuznetsov {
  template< class T >
  class Stack {
  public:
    Stack():
      list_()
    {}

    Stack(const Stack< T >& s):
      list_(s.list_)
    {}

    Stack(Stack< T >&& s) noexcept :
      list_(std::move(s.list_))
    {}

    Stack& operator=(const Stack< T >& s)
    {
      this->list_ = s.list_;
      return *this;
    }

    Stack& operator=(Stack< T >&& s) noexcept
    {
      if (this == &s) {
        return *this;
      }
      this->list_ = std::move(s.list_);
      return *this;
    }

    T& top()
    {
      return list_.front();
    }

    const T& top() const
    {
      return list_.front();
    }

    bool empty()
    {
      return list_.empty();
    }
    size_t size()
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

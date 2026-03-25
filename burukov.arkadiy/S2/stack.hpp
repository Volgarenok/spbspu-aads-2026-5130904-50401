#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"

namespace burukov
{
  template< class T >
  class Stack
  {
  public:
    T &top()
    {
      return list_.front();
    }

    const T &top() const
    {
      return list_.front();
    }

    bool empty() const
    {
      return list_.size() == 0;
    }

    size_t size() const
    {
      return list_.size();
    }

    void push(const T &value)
    {
      list_.pushFront(value);
    }

    void pop()
    {
      list_.popFront();
    }

    void swap(Stack &other)
    {
      list_.swap(other.list_);
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

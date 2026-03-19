#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"

namespace sedov
{
  template< class T >
  class Stack
  {
  public:
    Stack():
      list_()
    {}

    Stack(const Stack & s):
      list_(s.list_)
    {}

    Stack(Stack< T > && s):
      list_(std::move(s.list_))
    {}

    ~Stack() = default;

    Stack & operator=(const Stack< T > & s)
    {
      Stack< T > temp(s);
      swap(temp);
      return *this;
    }

    Stack & operator=(Stack< T > && s)
    {
      if (this == &s)
      {
        return *this;
      }
      list_ = std::move(s.list_);
      return *this;
    }

    T & top()
    {
      return list_.back();
    }

    const T & top() const
    {
      return list_.back();
    }

    bool empty() const
    {
      return list_.size() == 0;
    }

    size_t size() const
    {
      return list_.size();
    }

    void push(const T & v)
    {
      list_.pushBack(v);
    }

    void pop()
    {
      list_.popBack();
    }

    void swap(Stack & s)
    {
      list_.swap(s.list_);
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

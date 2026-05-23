#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"
#include <cstddef>
#include <stdexcept>

namespace vasyakin
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& value);
    void pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    vasyakin::List< T > list_;
  };

  template< class T >
  void Stack< T >::push(const T& value)
  {
    list_.insert(list_.end(), value);
  }

  template< class T >
  void Stack< T >::pop()
  {
    if (list_.getSize() == 0)
    {
      throw std::runtime_error ("Stack is empty");
    }
    list_.erase(list_.end());
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (list_.getSize() == 0)
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    if (list_.getSize() == 0)
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.getSize() == 0;
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.getSize();
  }
}

#endif

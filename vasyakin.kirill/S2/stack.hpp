#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <stdexcept>
#include "list.hpp"

namespace vasyakin
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& value);
    void push(T&& value);

    void pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;

    template< class... Args >
    void emplace(Args&&... args);
  private:
    vasyakin::List< T > list_;
  };

  template< class T >
  void Stack< T >::push(const T& value)
  {
    list_.emplace_front(value);
  }

  template< class T >
  void Stack< T >::push(T&& value)
  {
    list_.emplace_front(std::move(value));
  }

  template< class T >
  void Stack< T >::pop()
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error ("Stack is empty");
    }
    list_.erase(list_.end());
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.size() == 0;
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  template< class... Args >
  void Stack< T >::emplace(Args&&... args)
  {
    list_.emplace_front(std::forward< Args >(args)...);
  }
}

#endif

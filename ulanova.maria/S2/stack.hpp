#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <utility>

#include <list.hpp>

namespace ulanova
{
  template < class T >
  class Stack
  {
  public:
    void push(const T& value);
    void push(T&& value);
    void clear() noexcept;
    void pop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
  private:
    List< T > list_;
  };

  template < class T >
  void Stack< T >::push(const T& value)
  {
    list_.push_front(value);
  }
  template< class T >
  void Stack< T >::push(T&& value)
  {
    list_.push_front(std::forward< T >(value));
  }
  template < class T >
  void Stack< T >::clear() noexcept
  {
    list_.clear();
  }
  template < class T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    list_.pop_front();
  }
  template < class T >
  T& Stack< T >::front()
  {
    if(empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return list_.front();
  }
  template < class T >
  const T& Stack< T >::front() const
  {
    if(empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return list_.front();
  }
  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }
}
#endif

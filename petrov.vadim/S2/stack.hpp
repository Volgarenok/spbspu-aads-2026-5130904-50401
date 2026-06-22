#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <cstddef>
#include <utility>
#include "../common/List/list_struct.hpp"

namespace petrov
{
  template< typename T >
  class Stack {
  public:
    Stack() = default;
    Stack(const Stack< T >& s) = default;
    Stack(Stack< T >&& s) noexcept = default;

    Stack< T >& operator=(const Stack< T >& s) = default;
    Stack< T >& operator=(Stack< T >&& s) noexcept = default;
    ~Stack() = default;

    template< typename U >
    void push(U&& rhs);

    const T& top() const;
    T& top();

    void pop();

    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;

  private:
    List< T > list_;
  };

  template< typename T >
  template< typename U >
  void Stack< T >::push(U&& rhs)
  {
    list_.push_front(std::forward< U >(rhs));
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    list_.pop_front();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.size() == 0;
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< typename T >
  void Stack< T >::clear() noexcept
  {
    list_.clear();
  }
}

#endif

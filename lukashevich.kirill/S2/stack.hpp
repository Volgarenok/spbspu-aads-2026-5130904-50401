#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <list/list.hpp>

namespace lukashevich
{
  template< class T >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack< T >& stack) = default;
      Stack(Stack< T >&& stack) = default;
      ~Stack() = default;

      Stack< T >& operator=(const Stack< T >& stack) = default;
      Stack< T >& operator=(Stack< T >&& stack) = default;

      bool empty() const noexcept;
      size_t size() const noexcept;
      T& top();
      const T& top() const;
      void clear();
      void push(const T& rhs);
      void push(T&& rhs);
      void pop();

    private:
      List< T > list_;
  };

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (empty()) {
      throw std::runtime_error("empty stack");
    }
    return list_.back();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    if (empty()) {
      throw std::runtime_error("empty stack");
    }
    return list_.back();
  }

  template< class T >
  void Stack< T >::clear()
  {
    list_.clear();
  }

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    list_.pushBack(rhs);
  }

  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    list_.pushBack(std::move(rhs));
  }

  template< class T >
  void Stack< T >::pop()
  {
    if (empty()) {
      throw std::runtime_error("empty stack");
    }
    list_.popBack();
  }
}
#endif

#ifndef YALOVSKY_STACK_HPP
#define YALOVSKY_STACK_HPP

#include <cstddef>

#include "../common/list.hpp"

namespace yalovsky
{
  template< class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    T drop();
    void pop() noexcept;
    void clear() noexcept;
    void swap(Stack< T >& other) noexcept;

  private:
    List< T > list_;
  };

  template< class T >
  T& Stack< T >::top()
  {
    return list_.back();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    return list_.back();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  std::size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    list_.pushBack(value);
  }

  template< class T >
  T Stack< T >::drop()
  {
    T value = top();
    pop();
    return value;
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    list_.popBack();
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    list_.clear();
  }

  template< class T >
  void Stack< T >::swap(Stack< T >& other) noexcept
  {
    list_.swap(other.list_);
  }
}

#endif

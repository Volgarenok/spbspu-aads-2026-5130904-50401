#ifndef STACK_HPP
#define STACK_HPP

#include <utility>
#include "bidir_list.hpp"

namespace tarasenko
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void pop();
    const T& top() const;
    T& top();
    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    BidirList< T > data_;
  };

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    data_.push_front(rhs);
  }

  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    data_.push_front(std::forward< T >(rhs));
  }

  template< class T >
  void Stack< T >::pop()
  {
    data_.pop_front();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    return data_.front();
  }

  template< class T >
  T& Stack< T >::top()
  {
    return data_.front();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    data_.clear();
  }
}

#endif

#ifndef STACK_HPP
#define STACK_HPP

#include "bidir_list.hpp"

namespace tarasenko
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    T pop();
    const T& top() const;
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
    data_.push_front(std::move(rhs));
  }

  template< class T >
  T Stack< T >::pop()
  {
    T value = data_.front();
    data_.pop_front();
    return value;
  }

  template< class T >
  const T& Stack< T >::top() const
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

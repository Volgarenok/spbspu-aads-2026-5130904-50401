#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <utility>
#include "bidir_list.hpp"

namespace tarasenko
{
  template< class T >
  class Queue
  {
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void pop();
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    BidirList< T > data_;
  };

  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    data_.push_back(rhs);
  }

  template< class T >
  void Queue< T >::push(T&& rhs)
  {
    data_.push_back(std::forward< T >(rhs));
  }

  template< class T >
  void Queue< T >::pop()
  {
    data_.pop_front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    return data_.front();
  }

  template< class T >
  T& Queue< T >::front()
  {
    return data_.front();
  }

  template< class T >
  const T& Queue< T >::back() const
  {
    return data_.back();
  }

  template< class T >
  T& Queue< T >::back()
  {
    return data_.back();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    data_.clear();
  }
}
#endif

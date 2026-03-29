#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "bidir_list.hpp"

namespace tarasenko
{
  template< class T >
  class Queue
  {
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    T pop();
    const T& front() const;
    const T& back() const;
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
    data_.push_back(std::move(rhs));
  }

  template< class T >
  T Queue< T >::pop()
  {
    T value = data_.front();
    data_.pop_front();
    return value;
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    return data_.front();
  }

  template< class T >
  const T& Queue< T >::back() const
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

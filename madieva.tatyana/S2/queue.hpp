#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <stdexcept>
#include <utility>
#include <list.hpp>

namespace madieva
{
  template< class T >
  class Queue {
  public:
    void push(const T & val);
    void push(T && val);
    template< class... Args >
    T & emplace(Args &&... args);
    T & front();
    const T & front() const;
    size_t size() const noexcept;
    void pop() noexcept;
    void clear() noexcept;
    bool empty() const noexcept;
  private:
    List< T > data_;
  };

  template< class T >
  void Queue< T >::push(const T & val)
  {
    data_.pushBack(val);
  }

  template< class T >
  void Queue< T >::push(T && val)
  {
    data_.pushBack(std::forward< T >(val));
  }

  template< class T >
  template< class... Args >
  T & Queue< T >::emplace(Args &&... args)
  {
    return data_.emplace_back(std::forward< Args >(args)...);
  }

  template< class T >
  T & Queue< T >::front()
  {
    if (!empty()) {
      LIter< T > it = data_.begin();
      return (*it);
    } else {
      throw std::runtime_error("Queue is empty");
    }
  }

  template< class T >
  const T & Queue< T >::front() const
  {
    if (!empty()) {
      LCIter< T > it = data_.begin();
      return (*it);
    } else {
      throw std::runtime_error("Queue is empty");
    }
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    data_.popFront();
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    data_.clear();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }
}

#endif

#ifndef YALOVSKY_QUEUE_HPP
#define YALOVSKY_QUEUE_HPP

#include <cstddef>
#include <utility>

#include "../common/list.hpp"

namespace yalovsky
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >& other) = default;
    Queue(Queue< T >&& other) noexcept = default;
    ~Queue() = default;

    Queue& operator=(const Queue< T >& other) = default;
    Queue& operator=(Queue< T >&& other) noexcept = default;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    T drop();
    void pop() noexcept;
    void clear() noexcept;
    void swap(Queue< T >& other) noexcept;

  private:
    List< T > list_;
  };

  template< class T >
  T& Queue< T >::front()
  {
    return list_.front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    return list_.front();
  }

  template< class T >
  T& Queue< T >::back()
  {
    return list_.back();
  }

  template< class T >
  const T& Queue< T >::back() const
  {
    return list_.back();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  std::size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  void Queue< T >::push(const T& value)
  {
    list_.pushBack(value);
  }

  template< class T >
  T Queue< T >::drop()
  {
    T value = front();
    pop();
    return value;
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    list_.popFront();
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    list_.clear();
  }

  template< class T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    list_.swap(other.list_);
  }
}

#endif

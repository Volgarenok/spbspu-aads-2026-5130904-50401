#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <utility>
#include <cstddef>

#include <list/list.hpp>

namespace lukashevich
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >& queue) = default;
    Queue(Queue< T >&& queue) = default;
    ~Queue() = default;

    Queue< T >& operator=(const Queue< T >& queue) = default;
    Queue< T >& operator=(Queue< T >&& queue) = default;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void clear();
    void push(const T& rhs);
    void push(T&& rhs);
    void pop();

  private:
    List< T > list_;
  };

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  T& Queue< T >::front()
  {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return list_.front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return list_.front();
  }

  template< class T >
  T& Queue< T >::back()
  {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return list_.back();
  }

  template< class T >
  const T& Queue< T >::back() const
  {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return list_.back();
  }

  template< class T >
  void Queue< T >::clear()
  {
    list_.clear();
  }

  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    list_.pushBack(rhs);
  }

   template< class T >
  void Queue< T >::push(T&& rhs)
  {
    list_.pushBack(std::move(rhs));
  }

  template< class T >
  void Queue< T >::pop()
  {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    list_.popFront();
  }
}
#endif

#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include "list.hpp"
#include <utility>

namespace hvostov {
  template< class T >
  class Queue {
    public:
      Queue();
      Queue(const Queue< T >& queue);
      Queue(Queue< T >&& queue) noexcept;
      ~Queue() = default;
      Queue< T >& operator=(const Queue< T >& queue);
      Queue< T >& operator=(Queue< T >&& queue) noexcept;

      T drop();
      void push(T rhs);
      void clear() noexcept;
      bool empty() const noexcept;
      T& front();
      const T& front() const;
      T& back();
      const T& back() const;
      size_t getSize() const noexcept;
    private:
      List< T > list_;
      Liter< T > tail_;
  };
}

template< class T >
hvostov::Queue< T >::Queue():
  list_(),
  tail_()
{
  tail_ = list_.begin();
}

template< class T >
hvostov::Queue< T >::Queue(const Queue< T >& queue):
  list_(queue.list_),
  tail_(queue.tail_)
{}

template< class T >
hvostov::Queue< T >::Queue(Queue< T >&& queue) noexcept:
  list_(std::move(queue.list_)),
  tail_(std::move(queue.tail_))
{}

template< class T >
hvostov::Queue< T >& hvostov::Queue< T >::operator=(const Queue< T >& queue)
{
  if (this == &queue) {
    return *this;
  }
  list_ = queue.list_;
  tail_ = queue.tail_;
  return *this;
}

template< class T >
hvostov::Queue< T >& hvostov::Queue< T >::operator=(Queue< T >&& queue) noexcept
{
  if (this == &queue) {
    return *this;
  }
  list_ = std::move(queue.list_);
  tail_ = std::move(queue.tail_);
  return *this;
}

#endif

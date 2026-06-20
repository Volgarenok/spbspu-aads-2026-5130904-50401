#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include "list.hpp"

namespace hvostov {
  template< class T >
  class Queue {
  public:
    Queue();
    Queue(const Queue< T >& queue) = default;
    Queue(Queue< T >&& queue) noexcept = default;
    ~Queue() = default;
    Queue< T >& operator=(const Queue< T >& queue) = default;
    Queue< T >& operator=(Queue< T >&& queue) noexcept = default;

    void pop() noexcept;

    void push(const T& rhs);
    void push(T&& rhs);
    template< class... Args >
    void emplace(Args&&... args);

    void clear() noexcept;
    bool empty() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    size_t size() const noexcept;

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
void hvostov::Queue< T >::pop() noexcept
{
  list_.eraseAfter(list_.end());
  if (empty()) {
    tail_ = list_.begin();
  }
}

template< class T >
template< class... Args >
void hvostov::Queue< T >::emplace(Args&&... args)
{
  tail_ = list_.emplace(tail_, std::forward< Args >(args)...);
  if (list_.size() == 1) {
    tail_ = list_.begin();
  }
}

template< class T >
void hvostov::Queue< T >::push(const T& rhs)
{
  tail_ = list_.insertAfter(tail_, rhs);
}

template< class T >
void hvostov::Queue< T >::push(T&& rhs)
{
  tail_ = list_.insertAfter(tail_, std::forward< T >(rhs));
}

template< class T >
void hvostov::Queue< T >::clear() noexcept
{
  list_.clear();
  tail_ = list_.begin();
}

template< class T >
bool hvostov::Queue< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
T& hvostov::Queue< T >::front() noexcept
{
  return *(list_.begin());
}

template< class T >
const T& hvostov::Queue< T >::front() const noexcept
{
  return *(list_.begin());
}

template< class T >
size_t hvostov::Queue< T >::size() const noexcept
{
  return list_.size();
}

#endif

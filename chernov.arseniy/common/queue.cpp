#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <list.hpp>

namespace chernov {
  template< class T >
  class Queue {
  public:
    Queue() = default;
    Queue(const Queue< T > &) = default;
    Queue(Queue< T > &&) = default;
    ~Queue() = default;
    Queue< T > & operator=(const Queue< T > &) = default;
    Queue< T > & operator=(Queue< T > &&) = default;

    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  private:
    List< T > list_;
  };
}

template< class T >
bool chernov::Queue< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t chernov::Queue< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void chernov::Queue< T >::clear()
{
  list_.clear();
}

#endif

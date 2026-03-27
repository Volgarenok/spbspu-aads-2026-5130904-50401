#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace zharov {
  template< class T >
  class Queue {
    List< T > list_;
  public:
    Queue();
    Queue(const Queue & q);
    Queue(Queue && q) noexcept;
    ~Queue() = default;
    Queue & operator=(const Queue & q);
    Queue & operator=(Queue && q) noexcept;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
    void swap(Queue & q);
  };
}

template< class T >
zharov::Queue< T >::Queue():
  list_()
{}

template< class T >
zharov::Queue< T >::Queue(const Queue & q):
  list_(q.list_)
{}

template< class T >
zharov::Queue< T >::Queue(Queue && q) noexcept:
  list_(std::move(q.list_))
{}

template< class T >
zharov::Queue< T > & zharov::Queue< T >::operator=(const Queue & q)
{
  if (this != std::addressof(q)) {
      Queue< T > temp(q);
      swap(temp);
    }
    return *this;
}

template< class T >
zharov::Queue< T > & zharov::Queue< T >::operator=(Queue && q) noexcept
{
  if (this != std::addressof(q)) {
    clear();
    list_ = std::move(q.list_);
  }
  return *this;
}

template< class T >
T & zharov::Queue< T >::front()
{
  return list_.front();
}

template< class T >
const T & zharov::Queue< T >::front() const
{
  return list_.front();
}

template< class T >
T & zharov::Queue< T >::back()
{
  return list_.back();
}

template< class T >
const T & zharov::Queue< T >::back() const
{
  return list_.back();
}

template< class T >
bool zharov::Queue< T >::empty() const
{
  return !list_.size();
}

template< class T >
size_t zharov::Queue< T >::size() const
{
  return list_.size();
}

template< class T >
void zharov::Queue< T >::clear()
{
  list_.clear();
}

template< class T >
void zharov::Queue< T >::push(T rhs)
{
  list_.pushBack(rhs);
}

template< class T >
T zharov::Queue< T >::drop()
{
  T fr = front();
  list_.popFront();
  return fr;
}

template< class T >
void zharov::Queue< T >::swap(Queue & q)
{
  list_.swap(q.list_);
}

#endif

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <iterator>

#include <list.hpp>
#include <liter.hpp>

namespace chernov {
  template< class T >
  class Queue {
  public:
    Queue();
    Queue(const Queue< T > & queue);
    Queue(Queue< T > && queue);
    ~Queue() = default;
    Queue< T > & operator=(const Queue< T > & queue);
    Queue< T > & operator=(Queue< T > && queue);

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    void push(const T & value);
    void push(T && value);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  private:
    List< T > list_;
    LIter< T > last_;
    void updateLast();
  };
}

template< class T >
void chernov::Queue< T >::updateLast()
{
  if (list_.empty()) {
    last_ = list_.beforeBegin();
  } else {
    LIter< T > iter = list_.begin();
    LIter< T > next_iter = iter;
    ++next_iter;
    while (next_iter != list_.begin()) {
      ++iter;
      ++next_iter;
    }
    last_ = iter;
  }
}

template< class T >
chernov::Queue< T >::Queue():
  list_(),
  last_()
{
  updateLast();
}

template< class T >
chernov::Queue< T >::Queue(const Queue< T > & queue):
  list_(queue.list_),
  last_()
{
  updateLast();
}

template< class T >
chernov::Queue< T >::Queue(Queue< T > && queue):
  list_(std::move(queue.list_)),
  last_()
{
  updateLast();
}

template< class T >
chernov::Queue< T > & chernov::Queue< T >::operator=(const Queue< T > & queue)
{
  if (this == &queue) {
    return *this;
  }
  list_ = queue.list_;
  updateLast();
  return *this;
}

template< class T >
chernov::Queue< T > & chernov::Queue< T >::operator=(Queue< T > && queue)
{
  if (this == &queue) {
    return *this;
  }
  list_ = std::move(queue.list_);
  updateLast();
  return *this;
}

template< class T >
T & chernov::Queue< T >::front()
{
  return list_.first();
}

template< class T >
const T & chernov::Queue< T >::front() const
{
  return list_.first();
}

template< class T >
T & chernov::Queue< T >::back()
{
  return *last_;
}

template< class T >
const T & chernov::Queue< T >::back() const
{
  return *last_;
}

template< class T >
void chernov::Queue< T >::push(const T & value)
{
  last_ = list_.insertAfter(last_, value);
}

template< class T >
void chernov::Queue< T >::push(T && value)
{
  last_ = list_.insertAfter(last_, std::move(value));
}

template< class T >
void chernov::Queue< T >::pop()
{
  if (list_.size() == 1) {
    list_.popFront();
    last_ = list_.beforeBegin();
  } else {
    list_.popFront();
  }
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

#ifndef MY_QUEUE
#define MY_QUEUE

#include "list.hpp"
#include <stdexcept>
#include <utility>

namespace levkin {
  template < typename T > class Queue
  {
  private:
    levkin::List< T > list_;

  public:
    Queue() = default;

    T& front();
    const T& front() const;

    bool empty() const;
    size_t size() const;

    void push(const T& v);
    void push(T&& v);
    void pop();
    T drop();
  };

  template < class T > bool Queue< T >::empty() const
  {
    return list_.size() == 0;
  }

  template < class T > size_t Queue< T >::size() const { return list_.size(); }

  template < class T > T& Queue< T >::front()
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty!");
    }
    return *list_.begin();
  }

  template < class T > const T& Queue< T >::front() const
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty!");
    }
    return *list_.cbegin();
  }

  template < class T > void Queue< T >::push(const T& v) { list_.pushBack(v); }

  template < class T > void Queue< T >::push(T&& v)
  {
    list_.pushBack(std::move(v));
  }

  template < class T > void Queue< T >::pop()
  {
    if (!empty()) {
      list_.popFront();
    }
  }

  template < class T > T Queue< T >::drop()
  {
    if (empty()) {
      throw std::out_of_range("queue is empty!");
    }
    T val = std::move(front());
    list_.popFront();
    return val;
  }
}

#endif

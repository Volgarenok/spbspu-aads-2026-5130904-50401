#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP
#include "../Common/mylist.hpp"

namespace goltsov
{
  template< typename T >
  class Queue
  {
    List< T > dates_;
    LIter< T > tail_;
    size_t size_;
  public:
    Queue();
    ~Queue() = default;
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);
    Queue< T >& operator=(const Queue< T >& other);
    Queue< T >& operator=(Queue< T >&& other);

    void push(const T& rhs);
    void push(T&& rhs);
    void drop();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    bool empty() const;
    size_t size() const;
  };
}

namespace goltsov
{
  template< typename T >
  Queue< T >::Queue():
    size_(0)
  {}
  template< typename T >
  Queue< T >::Queue(const Queue< T >& other):
    dates_(other.dates_),
    size_(other.size_)
  {
    tail_ = dates_.begin();
    while (tail_.hasNext() && tail_.next().hasNext())
    {
      tail_ = tail_.next();
    }
  }
  template< typename T >
  Queue< T >::Queue(Queue< T >&& other):
    dates_(std::move(other.dates_)),
    size_(other.size_)
  {
    tail_ = dates_.begin();
    while (tail_.hasNext() && tail_.next().hasNext())
    {
      tail_ = tail_.next();
    }
  }
  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    dates_ = other.dates_;
    size_ = other.size_;
    tail_ = dates_.begin();
    while (tail_.hasNext() && tail_.next().hasNext())
    {
      tail_ = tail_.next();
    }
    return * this;
  }
  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other)
  {
    dates_ = std::move(other.dates_);
    size_ = other.size_;
    tail_ = dates_.begin();
    while (tail_.hasNext() && tail_.next().hasNext())
    {
      tail_ = tail_.next();
    }
    return * this;
  }

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    tail_ = dates_.insert(tail_, rhs);
    size_ += 1;
  }
  template< typename T >
  void Queue< T >::push(T&& rhs)
  {
    tail_ = dates_.insert(tail_, std::move(rhs));
    size_ += 1;
  }
  template< typename T >
  void Queue< T >::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    dates_.pop_start();
    size_--;
    if (empty())
    {
      tail_ = LIter< T >();
    }
  }
  template< typename T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (* dates_.begin());
  }
  template< typename T >
  const T& Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (* dates_.begin());
  }
  template< typename T >
  T& Queue< T >::back()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (* tail_);
  }
  template< typename T >
  const T& Queue< T >::back() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return (* tail_);
  }
  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }
}
#endif

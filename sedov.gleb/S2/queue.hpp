#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace sedov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T > & q) = default;
    Queue(Queue< T > && q) = default;
    ~Queue() = default;

    Queue & operator=(const Queue< T > & q) = default;
    Queue & operator=(Queue< T > && q) = default;

    T & front();
    const T & front() const;

    bool empty() const;
    size_t size() const;

    void push(const T& v);
    void push(T && v);
    void pop() noexcept;
    void swap(Queue< T > & q) noexcept;
    void clear() noexcept;
  private:
    List< T > list_;
  };

  template< class T >
  T & Queue< T >::front()
  {
    return list_.front();
  }

  template< class T >
  const T & Queue< T >::front() const
  {
    return list_.front();
  }

  template< class T >
  bool Queue< T >::empty() const
  {
    return list_.size() == 0;
  }

  template< class T >
  size_t Queue< T >::size() const
  {
    return list_.size();
  }

  template< class T >
  void Queue< T >::push(const T& v)
  {
    list_.pushBack(v);
  }

  template< class T >
  void Queue< T >::push(T && v)
  {
    list_.pushBack(std::move(v));
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    list_.popFront();
  }

  template< class T >
  void Queue< T >::swap(Queue< T > & q) noexcept
  {
    list_.swap(q.list_);
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    list_.clear();
  }
}
#endif

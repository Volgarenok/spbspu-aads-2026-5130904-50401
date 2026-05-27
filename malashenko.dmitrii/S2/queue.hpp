#ifndef QUEUE
#define QUEUE
#include <list/list.hpp>

namespace malashenko
{
  template< class T >
  class Queue {
  public:
    Queue() = default;
    Queue(const Queue< T >& other) = default;
    Queue(Queue< T >&& other) = default;

    Queue& operator=(const Queue< T >& other) = default;
    Queue& operator=(Queue< T >&& other) = default;

    ~Queue() = default;

    T& front();
    const T& front() const;


    bool empty() const;
    size_t size() const;


    void push(const T& value);
    void push(T&& value);
    void pop();
    void swap(Queue< T >& other);
    void clear();

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
  bool Queue< T >::empty() const
  {
    return list_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const
  {
    return list_.size();
  }

  template< class T >
  void Queue< T >::push(const T& value)
  {
    list_.push_back(value);
  }

  template< class T >
  void Queue< T >::push(T&& value)
  {
    list_.push_back(value);
  }

  template< class T >
  void Queue< T >::pop()
  {
    list_.pop_front();
  }

  template< class T >
  void Queue< T >::swap(Queue< T >& other)
  {
    list_.swap(other.list_);
  }

  template< class T >
  void Queue< T >::clear()
  {
    list_.clear();
  }
}

#endif

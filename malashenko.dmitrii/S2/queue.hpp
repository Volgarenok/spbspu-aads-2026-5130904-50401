#ifndef QUEUE
#define QUEUE
#include <list/list.hpp>

namespace malashenko
{
  template< class T >
  class Queue {
  public:
    Queue();
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);

    Queue& operator=(const Queue< T >& other);
    Queue& operator=(Queue< T >&& other);

    T& front() const;
    T& back() const;

    bool empty() const;
    size_t size() const;


    void push(const T& value);
    void pop();
    void swap(Queue< T >& other);
    void clear();

    ~Queue();
  private:
    List< T > list_;
  };

  template< class T >
  Queue< T >::Queue():
    list_()
  {}

  template< class T >
  Queue< T >::Queue(const Queue< T >& other):
    list_(other.list_)
  {}

  template< class T >
  Queue< T >::Queue(Queue< T >&& other):
    list_(std::move(other.list_))
  {}

  template< class T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    Queue< T > temp(other);
    swap(temp);
    return *this;
  }

  template< class T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    list_ = std::move(other.list_);
    return *this;
  }

  template< class T >
  T& Queue< T >::front() const
  {
    return list_.front();
  }

  template< class T >
  T& Queue< T >::back() const
  {
    return list_.back();
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

  template< class T >
  Queue< T >::~Queue()
  {
    list_.clear();
  }

}

#endif

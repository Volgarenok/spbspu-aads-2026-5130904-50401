#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../Common/list.hpp"

namespace kondrat
{
  template< class T >
  class Queue
  {
    public:
      Queue();
      ~Queue() = default;
      Queue(const Queue< T > & queue);
      Queue< T > & operator=(const Queue< T > & queue);
      Queue(Queue< T > && queue);
      Queue< T > & operator=(Queue< T > && queue);

      bool empty() const;
      size_t size() const;

      T & first() const;
      T & last() const;

      void push(const T & rhs);
      T drop();
      void clear();

    private:
      List< T > list_;
  };

  template< class T >
  Queue::Queue() : list_() {}

  template< class T >
  Queue::Queue(const Queue< T > & queue) : list_ (queue.list_);

  template< class T >
  Queue< T > & Queue< T >::operator=(const Queue< T > & queue)
  {
    list_ = queue.list_;
    return *this;
  }

  template< class T >
  Queue::Queue(Queue< T > && queue) : list_(std::move(queue.list_)) {}

  template< class T >
  Queue< T > & Queue::operator=(Queue< T > && queue)
  {
    if (this != &queue)
    {
      list_ = std::move(queue.list_);
    }
    return *this;
  }

  template< class T >
  bool Queue::empty() const
  {
    return list_.empty();
  }

  template< class T >
  size_t Queue::size() const
  {
    return list_.size();
  }

  template< class T >
  T & Queue::first() const
  {
    if (empty())
    {
      return std::runtime_error("empty queue");
    }
    return list_.front();
  }

  template< class T >
  T & Queue::last() const
  {
    if (empty())
    {
      throw std::runtime_error("empty queue")
    }
    return list_.back();
  }

  template< class T >
  Queue::push(const T & rhs)
  {
    list_.pushBack(rhs);
  }

  template< class T >
  T Queue::drop()
  {
    if (empty())
    {
      throw std::runtime_error("empty queue");
    }
    
    T value = list_.front();
    list_.popFront;
    return value;
  }
  
  template< class T >
  void clear();
  {
    list_.clear();
  }
}

#endif
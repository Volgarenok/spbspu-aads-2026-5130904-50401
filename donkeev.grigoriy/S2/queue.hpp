#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
#include <initializer_list>
#include <stdexcept>
namespace donkeev
{
  template< class T >
  class Queue
  {
    List< T > list_;

  public:
    Queue() = default;
    explicit Queue(std::initializer_list< T >);

    void push(const T&);
    void pop();
    
    T& front();
    const T& front() const;

    size_t size() const;
    bool isEmpty() const;
    void clear();

    ~Queue() = default;
  };

  template< class T >
  Queue< T >::Queue(std::initializer_list< T > il):
    list_()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      list_.pushBack(*it);
    }
  }

  template< class T >
  void Queue< T >::push(const T& val)
  {
    list_.pushBack(val);
  }
  template< class T >
  void Queue< T >::pop()
  {
    if (list_.isEmpty())
    {
      throw std::underflow_error("Empty queue");
    }
    list_.popFront();
  }

  template< class T >
  T& Queue< T >::front()
  {
    if (list_.isEmpty())
    {
      throw std::logic_error("Empty queue");
    }
    return *list_.begin();
  }
  template< class T >
  const T& Queue< T >::front() const
  {
    if (list_.isEmpty())
    {
      throw std::logic_error("Empty queue");
    }
    return *list_.cbegin();
  }

  template< class T >
  size_t Queue< T >::size() const
  {
    return list_.size();
  }
  template< class T >
  bool Queue< T >::isEmpty() const
  {
    return list_.isEmpty();
  }
  template< class T >
  void Queue< T >::clear()
  {
    list_.clearAll();
  }
}
#endif

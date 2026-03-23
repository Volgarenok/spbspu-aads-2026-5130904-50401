#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
#include <cstddef>
#include <cctype>
#include <stdexcept>

namespace vasyakin
{
  template< class T >
  class Queue
  {
  private:
    vasyakin::List< T > list;
  public:
    void push(T rhs);
    T drop();
    T peek() const;
    bool empty() const;
    size_t size() const;
  };

  template< class T >
  T Queue< T >::peek() const
  {
    if (list.get_size() == 0)
    {
      throw std::runtime_error("Queue is empty");
    }
    return *list.begin();
  }

  template< class T >
  void Queue< T >::push(T rhs)
  {
    list.push_back(rhs);
  }

  template< class T >
  T Queue< T >::drop()
  {
    if (list.get_size() == 0)
    {
      throw std::runtime_error("Queue is empty");
    }
    T val = *list.begin();
    list.erase(list.get_fake());
    return val;
  }

  template< class T >
  bool Queue< T >::empty() const
  {
    return list.get_size() == 0;
  }

  template< class T >
  size_t Queue< T >::size() const
  {
    return list.get_size();
  }
}

#endif

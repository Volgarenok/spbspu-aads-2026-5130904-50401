#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "../common/list.hpp"
#include "../common/node.hpp"
#include "../common/iterator.hpp"

namespace zhuravleva
{
  template<class T>
  class Queue
  {
  private:
    List<T> data;

  public:
    void push(const T& value);
    void pop();
    T front();
    bool empty() const;
    void clear();
  };
}

template< class T >
void zhuravleva::Queue< T >::push(const T& value)
{
  data.addEnd(value);
}


template< class T >
void zhuravleva::Queue< T >::pop()
{
  if (data.empty())
  {
    throw std::runtime_error("empty data error");
  }
  data.deleteStart();
}

template< class T >
T zhuravleva::Queue< T >::front()
{
  if (data.empty())
  {
    throw std::runtime_error("error");
  }
  returnd (*data.begin());
}

template< class T >
bool zhuravleva::Queue< T >::empty() const
{
  return data.empty();
}

template< class T >
void zhuravleva::Queue< T >::clear()
{
  data.clear();
}

#endif

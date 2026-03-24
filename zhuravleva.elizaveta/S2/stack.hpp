#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "common/list.hpp"
#include "../common/node.hpp"
#include "../common/iterator.hpp"

namespace zhuravleva
{
  template<class T>
  class Stack
  {
  private:
    List<T> data;

  public:
    void push(const T& value);
    void pop();
    T top();
    bool empty() const;
    void clear();
  };
}

template< class T >
void zhuravleva::Stack< T >::push(const T& value)
{
  data.addEnd(value);
}

template< class T >
void zhuravleva::Stack< T >::pop()
{
  if(data.empty())
  {
    throw std::runtime_error("empty data error");
  }
  data.deleteEnd();
}

template< class T >
T zhuravleva::Stack< T >::top()
{
  if(data.empty())
  {
    throw std::runtime_error("empty data error");
  }
  auto cur = data.begin();
  while(cur -> next != data.end())
  {
    cur = cur -> next;
  }
  return *cur;
}

template< class T >
bool zhuravleva::Stack< T >::empty() const
{
  return data.empty();
}

template< class T >
void zhuravleva::Stack< T >::clear()
{
  data.clear();
}

#endif
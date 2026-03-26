#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../common/list.hpp"
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
    T& top();
    T drop();
    bool empty() const;
    void clear();
  };
}

template< class T >
void zhuravleva::Stack< T >::push(const T& value)
{
  data.AddStart(value);
}

template< class T >
void zhuravleva::Stack< T >::pop()
{
  if(data.empty())
  {
    throw std::runtime_error("empty data error");
  }
  data.deleteStart();
}

template< class T >
T& zhuravleva::Stack< T >::top()
{
  if(data.empty())
  {
    throw std::runtime_error("empty data error");
  }
  return *data.begin();
}

template< class T >
T zhuravleva::Stack< T >::drop()
{
  if (data.empty())
  {
    throw std::runtime_error("empty data error when drop");
  }
  T val = *data.begin();
  data.deleteStart();
  return val;
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

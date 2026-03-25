#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace zharov {
  template< class T >
  class Stack {
    List< T > list_;
  public:
    Stack();
    Stack(const Stack & s);
    Stack(Stack && s) noexcept;
    ~Stack() = default;
    Stack & operator=(const Stack & s);
    Stack & operator=(Stack && s) noexcept;
    T & top();
    const T & top() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
    void swap(Stack & s);
  };
}

template< class T >
zharov::Stack< T >::Stack():
  list_()
{}

template< class T >
zharov::Stack< T >::Stack(const Stack & s):
  list_(s.list_)
{}

template< class T >
zharov::Stack< T >::Stack(Stack && s) noexcept:
  list_(std::move(s.list_))
{}

template< class T >
zharov::Stack< T > & zharov::Stack< T >::operator=(const Stack & s)
{
  if (this != std::addressof(s)) {
      Stack< T > temp(s);
      swap(temp);
    }
    return *this;
}

template< class T >
zharov::Stack< T > & zharov::Stack< T >::operator=(Stack && s) noexcept
{
  if (this != std::addressof(s)) {
    clear();
    list_ = std::move(s.list_);
  }
  return *this;
}

template< class T >
T & zharov::Stack< T >::top()
{
  return list_.back();
}

template< class T >
const T & zharov::Stack< T >::top() const
{
  return list_.back();
}

template< class T >
bool zharov::Stack< T >::empty() const
{
  return !list_.size();
}

template< class T >
size_t zharov::Stack< T >::size() const
{
  return list_.size();
}

template< class T >
void zharov::Stack< T >::clear()
{
  list_.clear();
}

template< class T >
void zharov::Stack< T >::push(T rhs)
{
  list_.pushBack(rhs);
}

template< class T >
T zharov::Stack< T >::drop()
{
  T tp = top();
  list_.popBack();
  return tp;
}

template< class T >
void zharov::Stack< T >::swap(Stack & s)
{
  list_.swap(s.list_);
}

#endif

#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <list.hpp>

namespace chernov {
  template< class T >
  class Stack {
  public:
    Stack() = default;
    Stack(const Stack< T > &) = default;
    Stack(Stack< T > &&) = default;
    ~Stack() = default;
    Stack< T > & operator=(const Stack< T > &) = default;
    Stack< T > & operator=(Stack< T > &&) = default;

    T & top();
    const T & top() const;
    void push(const T & value);
    void push(T && value);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  private:
    List< T > list_;
  };
}

template< class T >
T & chernov::Stack< T >::top()
{
  return list_.first();
}

template< class T >
const T & chernov::Stack< T >::top() const
{
  return list_.first();
}

template< class T >
void chernov::Stack< T >::push(const T & value)
{
  list_.pushFront(value);
}

template< class T >
void chernov::Stack< T >::push(T && value)
{
  list_.pushFront(std::move(value));
}

template< class T >
void chernov::Stack< T >::pop()
{
  list_.popFront();
}

template< class T >
bool chernov::Stack< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t chernov::Stack< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void chernov::Stack< T >::clear()
{
  list_.clear();
}

#endif

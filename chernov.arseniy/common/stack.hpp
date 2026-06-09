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

    T & top() noexcept;
    const T & top() const noexcept;

    void push(const T & value);
    void push(T && value);
    void pop() noexcept;

    template< class... Args >
    void emplace(Args &&... args);

    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
  private:
    List< T > list_;
  };
}

template< class T >
T & chernov::Stack< T >::top() noexcept
{
  return list_.first();
}

template< class T >
const T & chernov::Stack< T >::top() const noexcept
{
  return list_.first();
}

template< class T >
void chernov::Stack< T >::push(const T & value)
{
  emplace(value);
}

template< class T >
void chernov::Stack< T >::push(T && value)
{
  emplace(std::forward< T >(value));
}

template< class T >
void chernov::Stack< T >::pop() noexcept
{
  list_.popFront();
}

template< class T >
template< class... Args >
void chernov::Stack< T >::emplace(Args &&... args)
{
  list_.emplaceFront(std::forward< Args >(args)...);
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
void chernov::Stack< T >::clear() noexcept
{
  list_.clear();
}

#endif

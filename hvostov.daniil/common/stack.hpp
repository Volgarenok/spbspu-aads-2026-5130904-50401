#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include "list.hpp"

namespace hvostov {
  template< class T >
  class Stack {
  public:
    Stack() = default;
    Stack(const Stack< T >& stack) = default;
    Stack(Stack< T >&& stack) noexcept = default;
    ~Stack() = default;
    Stack< T >& operator=(const Stack< T >& stack) = default;
    Stack< T >& operator=(Stack< T >&& stack) noexcept = default;

    void pop() noexcept;

    void push(const T& rhs);
    void push(T&& rhs);
    template< class... Args >
    void emplace(Args&&... args);

    void clear() noexcept;
    bool empty() const noexcept;
    T& top() noexcept;
    const T& top() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > list_;
  };
}

template< class T >
void hvostov::Stack< T >::pop() noexcept
{
  list_.eraseAfter(list_.end());
}

template< class T >
template< class... Args >
void hvostov::Stack< T >::emplace(Args&&... args)
{
  list_.emplaceFront(std::forward< Args >(args)...);
}

template< class T >
void hvostov::Stack< T >::push(const T& rhs)
{
  list_.insertAfter(list_.end(), rhs);
}

template< class T >
void hvostov::Stack< T >::push(T&& rhs)
{
  list_.insertAfter(list_.end(), std::forward< T >(rhs));
}

template< class T >
void hvostov::Stack< T >::clear() noexcept
{
  list_.clear();
}

template< class T >
bool hvostov::Stack< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
T& hvostov::Stack< T >::top() noexcept
{
  return *(list_.begin());
}

template< class T >
const T& hvostov::Stack< T >::top() const noexcept
{
  return *(list_.begin());
}

template< class T >
size_t hvostov::Stack< T >::size() const noexcept
{
  return list_.size();
}

#endif

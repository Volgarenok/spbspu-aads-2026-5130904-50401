#ifndef stack_HPP
#define stack_HPP
#include <cstddef>
#include "list.hpp"
#include <utility>

namespace hvostov {
  template< class T >
  class Stack {
    public:
      Stack();
      Stack(const Stack< T >& stack);
      Stack(Stack< T >&& stack) noexcept;
      ~Stack() = default;
      Stack< T >& operator=(const Stack< T >& stack);
      Stack< T >& operator=(Stack< T >&& stack) noexcept;

      T drop();
      void push(const T& rhs);
      void clear() noexcept;
      bool empty() const noexcept;
      T& top();
      const T& top() const;
      size_t getSize() const noexcept;
    private:
      List< T > list_;
  };
}

template< class T >
hvostov::Stack< T >::Stack():
  list_()
{}

template< class T >
hvostov::Stack< T >::Stack(const Stack< T >& stack):
  list_(stack.list_)
{}

template< class T >
hvostov::Stack< T >::Stack(Stack< T >&& stack) noexcept:
  list_(std::move(stack.list_))
{}

template< class T >
hvostov::Stack< T >& hvostov::Stack< T >::operator=(const Stack< T >& stack)
{
  if (this == &stack) {
    return *this;
  }
  list_ = stack.list_;
  return *this;
}

template< class T >
hvostov::Stack< T >& hvostov::Stack< T >::operator=(Stack< T >&& stack) noexcept
{
  if (this == &stack) {
    return *this;
  }
  list_ = std::move(stack.list_);
  return *this;
}

template< class T >
T hvostov::Stack< T >::drop()
{
  T t = top();
  list_.eraseAfter(list_.end());
  return t;
}

template< class T >
void hvostov::Stack< T >::push(const T& rhs)
{
  list_.insertAfter(list_.end(), rhs);
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
T& hvostov::Stack< T >::top()
{
  return *(list_.begin());
}

template< class T >
const T& hvostov::Stack< T >::top() const
{
  return *(list_.begin());
}

template< class T >
size_t hvostov::Stack< T >::getSize() const noexcept
{
  return list_.getSize();
}

#endif

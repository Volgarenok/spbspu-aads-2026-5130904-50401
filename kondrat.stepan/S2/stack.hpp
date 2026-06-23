#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <list/list.hpp>

namespace kondrat
{
  template< class T >
  class Stack
  {
    public:
      Stack() = default;
      ~Stack() = default;
      Stack(const Stack< T > & stack) = default;
      Stack< T > & operator=(const Stack< T > & stack) = default;
      Stack(Stack< T > && stack) = default;
      Stack< T > & operator=(Stack< T > && stack) = default;

      bool empty() const;
      size_t size() const;

      T & front();
      const T & front() const;

      void push(const T & rhs);
      void push(T && rhs);
      void pop();
      void clear();
    private:
      List< T > list_;
  };

  template< class T >
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const
  {
    return list_.size();
  }

  template< class T >
  T & Stack< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("empty stack");
    }
    return list_.back();
  }

  template< class T >
  const T & Stack< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("empty stack");
    }
    return list_.back();
  }

  template< class T >
  void Stack< T >::push(const T & rhs)
  {
    list_.pushBack(rhs);
  }

  template< class T >
  void Stack< T >::push(T && rhs)
  {
    list_.pushBack(std::move(rhs));
  }

  template< class T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("empty stack");
    }
    list_.popBack();
  }

  template< class T >
  void Stack< T >::clear()
  {
    list_.clear();
  }
}

#endif

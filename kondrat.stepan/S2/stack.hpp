#ifndef STACK_HPP
#define STACK_HPP
#include "../Common/list.hpp"

namespace kondrat
{
  template< class T >
  class Stack
  {
    public:
      Stack();
      ~Stack() = default;
      Stack(const Stack< T > & stack);
      Stack< T > & operator=(const Stack< T > & stack);
      Stack(Stack< T > && stack);
      Stack< T > & operator=(Stack< T > && stack);

      bool empty() const;
      size_t size() const;

      T & first() const;

      void push(const T & rhs);
      T drop();
      void clear();

    private:
      List< T > list_
  };

  template< class T >
  Stack< T >::Stack() : list_() {}

  template< class T >
  Stack< T >::Stack(const Stack< T > & stack) : list_(stack.list_) {}

  template< class T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & stack)
  {
    list_ = stack.list_;
    return *this;
  }

  template< class T >
  Stack< T >::Stack(Stack< T > && stack) : list_(std::move(stack.list_)) {}

  template< class T >
  Stack< T > & Stack< T >::operator=(Stack< T > && stack)
  {
    if (this != &stack)
    {
      list_ = std::move(stack.list_)
    }
    return *this;
  }

  template< class T >
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const
  {
    return list_.size()
  }

  template< class T >
  T & Stack< T >::first() const
  {
    return list_.back();
  }

  template< class T >
  void Stack< T >::push(const T & rhs)
  {
    list_.pushBack(rhs);
  }

  template< class T >
  T Stack< T >::drop()
  {
    if (empty())
    {
      throw std::runtime_error("empty stack");
    }

    T value = list_.back();
    list_.popBack();
    return value;
  }

  template< class T >
  void Stack< T >::clear()
  {
    list_.clear();
  }
}

#endif
#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <initializer_list>
#include <stdexcept>

namespace donkeev
{
  template< class T >
  class Stack
  {
    List< T > list_;

  public:
    Stack() = default;
    explicit Stack(std::initializer_list< T >);

    void push(const T&);
    void pop();

    T& top();
    const T& top() const;

    bool isEmpty() const;
    size_t size() const;
    void clear();

    ~Stack() = default;
  };

  template< class T >
  Stack< T >::Stack(std::initializer_list< T > il):
    list_()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      list_.pushFront(*it);
    }
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    list_.pushFront(value);
  }
  template< class T >
  void Stack< T >::pop()
  {
    if (list_.isEmpty())
    {
      throw std::underflow_error("Empty queue");
    }
    list_.popFront();
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (list_.isEmpty())
    {
      throw std::underflow_error("Empty queue");
    }
    return *list_.begin();
  }
  template< class T >
  const T& Stack< T >::top() const
  {
    if (list_.isEmpty())
    {
      throw std::underflow_error("Empty queue");
    }
    return *list_.cbegin();
  }

  template< class T >
  bool Stack< T >::isEmpty() const
  {
    return list_.isEmpty();
  }
  template< class T >
  size_t Stack< T >::size() const
  {
    return list_.size();
  }
  template< class T >
  void Stack< T >::clear()
  {
    return list_.clearAll();
  }
}
#endif

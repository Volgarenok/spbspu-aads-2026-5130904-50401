#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>

namespace vasyakin
{
  template< class T >
  class Stack
  {
  private:
    vasyakin::List< T > list;
  public:
    void push(T rhs);
    T drop();
    T peek() const;
    bool empty() const;
    size_t size() const;
  };

  template< class T >
  T Stack< T >::peek() const
  {
    if (list.get_size() == 0)
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list.begin();
  }

  template< class T >
  void Stack< T >::push(T rhs)
  {
    list.insert(list.get_fake(), rhs);
  }

  template< class T >
  T Stack< T >::drop()
  {
    if (list.get_size() == 0)
    {
      throw std::runtime_error ("Stack is empty");
    }
    T val = *list.begin();
    list.erase(list.get_fake());
    return val;
  }

  template< class T >
  bool Stack< T >::empty() const
  {
    return list.get_size() == 0;
  }

  template< class T >
  size_t Stack< T >::size() const
  {
    return list.get_size();
  }
}

#endif

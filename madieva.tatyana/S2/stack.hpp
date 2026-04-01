#ifndef STAC_HPP
#define STAC_HPP
#include "../common/list.hpp"
#include <stdexcept>
namespace madieva {
  template< class T >
  class Stack {
    List< T > data;
  public:
    void push(const T & val);
    T & top();
    const T & top() const;
    size_t size() const noexcept;
    void pop();
    void clear() noexcept;
    bool empty() const noexcept;
  };


  template< class T >
  void Stack< T >::push(const T & val)
  {
    data.push_back(val);
  }

  template< class T >
  T & Stack< T >::top()
  {
    if (!empty()) {
      LIter< T > it = data.begin();
      --it;
      return (*it);
    } else {
      throw std::runtime_error("Stack is empty");
    }
  }

  template< class T >
  const T & Stack< T >::top() const
  {
    if (!empty()) {
      LCIter< T > it = data.begin();
      --it;
      return (*it);
    } else {
      throw std::runtime_error("Stack is empty");
    }
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return data.size();
  }

  template< class T >
  void Stack< T >::pop()
  {
    data.pop_back();
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    data.clear();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data.size() == 0;
  }
}

#endif

#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>

namespace studilova
{
  template< class T >
  class Stack
  {
    public:
      void push(const T& value);
      void pop();
      T& top();

      bool empty() const;
      size_t size() const;
    
    private:
      List< T > data_;
  };
}

template< class T >
void studilova::Stack< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< class T >
void studilova::Stack< T >::pop()
{
  if(empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  data_.popBack();
}

template< class T >
T& studilova::Stack< T >::top()
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  return data_.back();
}

template< class T >
bool studilova::Stack< T >::empty() const
{
  return data_.empty();
}

template< class T >
size_t studilova::Stack< T >::size() const{
  return data_.size();
}

#endif
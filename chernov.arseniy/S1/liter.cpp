#include "liter.hpp"

template< class T >
chernov::LIter< T >::LIter():
  ptr(nullptr)
{}

template< class T >
chernov::LIter< T >::LIter(Node< T > * node):
  ptr(node)
{}

template< class T >
bool chernov::LIter< T >::hasNext() const noexcept
{
  return ptr != nullptr;
}

template< class T >
chernov::LIter< T > chernov::LIter< T >::next() const
{
  return {ptr->next};
}

template< class T >
T & chernov::LIter< T >::value() const
{
  return ptr->data;
}

template< class T >
T & chernov::LIter< T >::operator*() const
{
  return ptr->data;
}

template< class T >
T * chernov::LIter< T >::operator->() const
{
  return &(ptr->data);
}

template< class T >
chernov::LIter< T > & chernov::LIter< T >::operator++()
{
  ptr = ptr->next;
  return *this;
}

template< class T >
chernov::LIter< T > chernov::LIter< T >::operator++(int)
{
  LIter old = *this;
  ++(*this);
  return old;
}

template< class T >
bool chernov::LIter< T >::operator==(const LIter< T > & other) const noexcept
{
  return ptr == other.ptr;
}

template< class T >
bool chernov::LIter< T >::operator!=(const LIter< T > & other) const noexcept
{
  return !(*this == other);
}

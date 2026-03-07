#include "lciter.hpp"

template< class T >
chernov::LCIter< T >::LCIter():
  ptr(nullptr)
{}

template< class T >
chernov::LCIter< T >::LCIter(const Node< T > * node):
  ptr(node)
{}

template< class T >
bool chernov::LCIter< T >::hasNext() const noexcept
{
  return ptr != nullptr;
}

template< class T >
chernov::LCIter< T > chernov::LCIter< T >::next() const
{
  return {ptr->next};
}

template< class T >
const T & chernov::LCIter< T >::value() const
{
  return ptr->data;
}

template< class T >
const T & chernov::LCIter< T >::operator*() const
{
  return ptr->data;
}

template< class T >
const T * chernov::LCIter< T >::operator->() const
{
  return &(ptr->data);
}

template< class T >
chernov::LCIter< T > & chernov::LCIter< T >::operator++()
{
  ptr = ptr->next;
  return *this;
}

template< class T >
chernov::LCIter< T > chernov::LCIter< T >::operator++(int)
{
  LCIter old = *this;
  ++(*this);
  return old;
}

template< class T >
bool chernov::LCIter< T >::operator==(const LCIter< T > & other) const noexcept
{
  return ptr == other.ptr;
}

template< class T >
bool chernov::LCIter< T >::operator!=(const LCIter< T > & other) const noexcept
{
  return !(*this == other);
}

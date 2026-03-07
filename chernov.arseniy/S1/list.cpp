#include "list.hpp"

template< class T >
chernov::Node< T > * chernov::List< T >::createFake()
{
  fake_ = static_cast< Node< T > * >(::operator new (sizeof(Node< T > *)));
  return fake_;
}

template< class T >
void chernov::List< T >::removeFake() noexcept
{
  ::operator delete(fake_);
}

template< class T >
chernov::List< T >::List():
  fake_(nullptr),
  size_(0)
{
  createFake();
}

template< class T >
chernov::List< T >::~List() noexcept
{
  removeFake();
}

template< class T >
size_t chernov::List< T >::size() noexcept
{
  return size_;
}

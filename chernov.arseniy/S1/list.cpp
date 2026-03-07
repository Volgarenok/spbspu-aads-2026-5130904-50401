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
  // ...
  removeFake();
}

template< class T >
chernov::List< T >::List(const List< T > & list):
  fake_(nullptr),
  size_(list.size_)
{
  createFake();
  // ...
}

template< class T >
chernov::List< T >::List(List< T > && list):
  fake_(list.fake_),
  size_(list.size_)
{
  list.fake_ = nullptr;
}

template< class T >
chernov::List< T > & chernov::List< T >::operator=(const List< T > & list)
{
  if (this == &list) {
    return *this;
  }
  // ...
  removeFake();
  // ...
  size_ = list.size_;
  return *this;
}

template< class T >
chernov::List< T > & chernov::List< T >::operator=(List< T > && list)
{
  if (this == &list) {
    return *this;
  }
  // destroy...
  fake_ = list.fake_;
  size_ = list.size_;
  list.fake_ = nullptr;
  return *this;
}

template< class T >
size_t chernov::List< T >::size() noexcept
{
  return size_;
}

#include "list.hpp"

template< class T >
hvostov::Node< T >* hvostov::List< T >::createFake()
{
  fake_ = static_cast< Node< T >* >(::operator new (sizeof(Node< T >*)));
  return fake_;
}

template< class T >
void hvostov::List< T >::rmFake() noexcept
{
  ::operator delete(fake_);
}

template< class T >
hvostov::List< T >::List():
  fake_(nullptr)
{
  createFake();
  fake_->next = fake_;
}

template< class T >
hvostov::List< T >::List(const List< T >& list):
  fake_(nullptr)
{
  createFake();
  fake_->next = fake_;
  Node< T >* head = fake_;
  for (Liter< T > it = list.begin(); it < it.end(); it++) {
    head = insertAfter(fake_, *it);
  }
}

template< class T >
hvostov::List< T >::List(List< T >&& list):
  fake_(list.fake_)
{
  list.fake_ = nullptr;
}

template< class T >
hvostov::List< T >::~List()
{
  clear();
  rmFake();
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(const List< T >& list)
{
  
  if (this == &list) {
    return *this;
  }
  clear();
  Node< T >* head = fake_;
  for (Liter< T > it = list.begin(); it < it.end(); it++) {
    head = insertAfter(fake_, *it);
  }
  return *this;
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(List< T >&& list)
{
  if (this == &list) {
    return *this;
  }
  clear();
  rmFake();
  fake_ = list.fake_;
  list.fake_ = nullptr;
  return *this;
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::begin() const noexcept
{
  return { fake_->next, fake_ };
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::end() const noexcept
{
  return { fake_, fake_ };
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::insertAfter(Liter< T > i, const T val)
{
  Node< T >* new_node = new Node< T >(val, i.curr_->next);
  i.curr_->next = new_node;
  return { new_node, fake_ };
}

template< class T >
void hvostov::List< T >::clear()
{
  while (fake_ != fake_) {
    Node< T >* tmp = fake_->next;
    delete fake_;
    fake_ = tmp;
  }
  fake_->next = fake_;
}

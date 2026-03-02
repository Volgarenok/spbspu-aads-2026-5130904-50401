#include "mylist.hpp"

using namespace goltsov;

template< class T >
bool LIter< T >::hasNext() const
{
  return ptr != nullptr && ptr->next != nullptr;
}
template< class T >
LIter< T > LIter< T >::next() const
{
  return {ptr->next};
}
template< class T >
T& LIter< T >::operator*() const
{
  return ptr->value;
}

template< class T >
bool LCIter< T >::hasNext() const
{
  return ptr != nullptr && ptr->next != nullptr;
}
template< class T >
LCIter< T > LCIter< T >::next() const
{
  return {ptr->next};
}
template< class T >
const T& LCIter< T >::operator*() const
{
  return {ptr->value};
}

template< class T >
Node< T >* List< T >::createFake()
{
  Node< T >* el = ::operator new(sizeof(Node< T >*));
  return el;
}
template< class T >
void List< T >::rmFake()
{
  ::operator delete(fake);
}
template< class T >
List< T >::List()
{
  fake = createFake();
}
template< class T >
List< T >::~List()
{
  clear();
}
template< class T >
List< T >::List(List< T >& other)
{
  fake = createFake();
  Node< T >* now_new = nullptr;
  Node< T >* now_old = other.fake->next;
  if (now_old != nullptr)
  {
    now_new = new Node< T > {now_old->value, nullptr};
    now_old = now_old->next;
  }
  fake->next = now_new;
  while(now_old != nullptr)
  {
    now_new->next = new Node< T > {now_old->value, nullptr};
    now_old = now_old->next;
  }
}
template< class T >
List< T >::List(List< T >&& other)
{
  fake = createFake();
  fake->next = other.fake->next;
  other.fake->next = nullptr;
}
template< class T >
List< T >& List< T >::operator=(const List< T >& other)
{
  fake = createFake();
  Node< T >* now_new = nullptr;
  Node< T >* now_old = other.fake->next;
  if (now_old != nullptr)
  {
    now_new = new Node< T > {now_old->value, nullptr};
    now_old = now_old->next;
  }
  fake->next = now_new;
  while(now_old != nullptr)
  {
    now_new->next = new Node< T > {now_old->value, nullptr};
    now_old = now_old->next;
  }
  return * this;
}
template< class T >
List< T >& List< T >::operator=(List< T >&& other)
{
  fake = createFake();
  fake->next = other.fake->next;
  other.fake->next = nullptr;
  return * this;
}
template< class T >
LIter<T> List<T>::begin()
{
  return {fake->next};
}
template< class T >
LCIter<T> List<T>::begin() const
{
  return {const_cast<const Node<T>*>(fake->next)}; 
}
template< class T >
LIter<T> List<T>::end()
{
  return {nullptr};
}
template< class T >
LCIter<T> List<T>::end() const
{
  return {nullptr};
}
template< class T >
void List< T >::push_start(const T& a)
{
  Node< T >* new_el = new Node< T >{a, fake->next};
  fake->next = new_el;
}
template< class T >
void List< T >::push_back(const T& a)
{
  if (fake->next == nullptr)
  {
    fake->next = new Node< T > {a, nullptr};
  }
  Node< T >* now = fake->next;
  while (now->next != nullptr)
  {
    now = now->next;
  }
  now->next = new Node< T > {a, nullptr};
}
template< class T >
void List< T >::pop_start()
{
  Node< T >* n = fake->next->next;
  delete fake->next;
  fake->next = n;
}
template< class T >
void List< T >::pop_end()
{
  Node< T >* now = fake;
  if (now->next == nullptr)
  {
    return;
  }
  while (now->next->next != nullptr)
  {
    now = now->next;
  }
  delete now->next;
  now->next = nullptr;
}
template< class T >
void List< T >::insert(LIter< T > i, const T& a)
{
  Node< T >* new_el = new Node< T > {a, i.next()};
  i.ptr->next = new_el;
}
template< class T >
void List< T >::clear()
{
  while (fake->next != nullptr)
  {
    Node< T >* temp = fake->next;
    fake->next = temp->next;
    delete temp;
  }
  rmFake();
}

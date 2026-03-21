#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>


namespace lukashevich
{
  template< class T >
  class Node
  {
    public:
      T val;
      Node< T >* next;
      Node< T >* prev;
  };

  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T >& list);
      List< T >& operator=(List< T >& list);
      List(const List< T >&& list);
      List< T >& operator=(List< T >&& list);

      size_t size() const;

    private:
      Node< T >* fake_;
      size_t size_;
  };

  template< class T >
  Node< T >* makeFake()
  {
    Node< T >* fake = static_cast< Node< T >* >(::operator new(sizeof(Node< T >)));
    fake->next = nullptr;
    fake->prev = nullptr;
    return fake;
  }

  template< class T >
  void removeFake(Node< T >* fake) noexcept
  {
    ::operator delete(fake);
  }
}

#endif

#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <cstddef>

namespace tarasenko
{
  template< class T >
  struct Node
  {
    T _val;
    Node* _next;
    Node* _prev;
  };

  template< class T >
  class BidirList;

  template< class T >
  class ListIter
  {
    friend class BidirList< T >;
    Node< T >* ptr;
  public:
    ListIter();
    bool operator==(const ListIter< T >& it) const;
  };

  template< class T >
  class ListConstIter
  {
    friend class BidirList< T >;
    Node< T >* ptr;
  public:
    ListConstIter();
    bool operator==(const ListConstIter< T >& it) const;
  };

  template< class T >
  class BidirList
  {
    Node< T >* _head;
    Node< T >* _tail;
    size_t _size;
  public:
    BidirList();
    ~BidirList();
    size_t size() const;
    ListIter< T > begin();
    ListConstIter< T > cbegin() const;
    ListIter< T > end();
    ListConstIter< T > cend() const;
  };
}

#endif

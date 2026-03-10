#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <cstddef>

namespace tarasenko
{
  template< class T >
  struct Node
  {
    T _val;
    Node< T >* _next;
    Node< T >* _prev;
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
    bool operator!=(const ListIter< T >& it) const;
  private:
    ListIter(Node< T >* node);
  };

  template< class T >
  class ListConstIter
  {
    friend class BidirList< T >;
    Node< T >* ptr;
  public:
    ListConstIter();
    bool operator==(const ListConstIter< T >& it) const;
    bool operator!=(const ListConstIter< T >& it) const;
  private:
    ListConstIter(Node< T >* node);
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
    void push_back(const T& val);
  };

  template< class T >
  BidirList< T >::BidirList() :
    _head(nullptr),
    _tail(nullptr),
    _size(0)
  {}

  template< class T >
  BidirList< T >::~BidirList()
  {
    Node< T >* current = _head;
    while(current)
    {
      Node< T >* next = current->_next;
      delete current;
      current = next;
    }
  }

  template< class T >
  size_t BidirList< T >::size() const
  {
    return _size;
  }

  template< class T >
  ListIter< T > BidirList< T >::begin()
  {
    return ListIter< T >(_head);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cbegin() const
  {
    return ListConstIter< T >(_head);
  }

   template< class T >
  ListIter< T > BidirList< T >::end()
  {
    return ListIter< T >(nullptr);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cend() const
  {
    return ListConstIter< T >(nullptr);
  }

  template< class T >
  ListIter< T >::ListIter() :
    ptr(nullptr)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter() :
    ptr(nullptr)
  {}

  template< class T >
  ListIter< T >::ListIter(Node< T >* node) :
    ptr(node)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(Node< T >* node) :
    ptr(node)
  {}

  template< class T >
  bool ListIter< T >::operator==(const ListIter< T >& it) const
  {
    return ptr == it.ptr;
  }

  template< class T >
  bool ListIter< T >::operator!=(const ListIter< T >& it) const
  {
    return !(ptr == it.ptr);
  }

  template< class T >
  bool ListConstIter< T >::operator==(const ListConstIter< T >& it) const
  {
    return ptr == it.ptr;
  }

  template< class T >
  bool ListConstIter< T >::operator!=(const ListConstIter< T >& it) const
  {
    return !(ptr == it.ptr);
  }

  template< class T >
  void BidirList< T >::push_back(const T& val)
  {
    if (_size)
    {
      Node< T >* elem = new Node< T >{val, nullptr, _tail};
      _tail->_next = elem;
      _tail = elem;
    }
    else
    {
      Node< T >* elem = new Node< T >{val, nullptr, nullptr};
      _head = elem;
      _tail = elem;
    }
    _size++;
  }
}

#endif

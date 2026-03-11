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
    const BidirList< T >* owner;
  public:
    ListIter();
    bool operator==(const ListIter< T >& it) const;
    bool operator!=(const ListIter< T >& it) const;
    T& operator*() const;
    T* operator->() const;
    ListIter< T >& operator++();
    ListIter< T > operator++(int);
    ListIter< T >& operator--();
    ListIter< T > operator--(int);
  private:
    ListIter(Node< T >* node, const BidirList< T >* list);
  };

  template< class T >
  class ListConstIter
  {
    friend class BidirList< T >;
    Node< T >* ptr;
    const BidirList< T >* owner;
  public:
    ListConstIter();
    bool operator==(const ListConstIter< T >& it) const;
    bool operator!=(const ListConstIter< T >& it) const;
    const T& operator*() const;
    const T* operator->() const;
    ListConstIter< T >& operator++();
    ListConstIter< T > operator++(int);
    ListConstIter< T >& operator--();
    ListConstIter< T > operator--(int);
  private:
    ListConstIter(Node< T >* node, const BidirList< T >* list);
  };

  template< class T >
  class BidirList
  {
    friend class ListIter< T >;
    friend class ListConstIter< T >;
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
    void push_front(const T& val);
    bool empty();
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
    return ListIter< T >(_head, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cbegin() const
  {
    return ListConstIter< T >(_head, this);
  }

   template< class T >
  ListIter< T > BidirList< T >::end()
  {
    return ListIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cend() const
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListIter< T >::ListIter() :
    ptr(nullptr),
    owner(nullptr)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter() :
    ptr(nullptr),
    owner(nullptr)
  {}

  template< class T >
  ListIter< T >::ListIter(Node< T >* node, const BidirList< T >* list) :
    ptr(node),
    owner(list)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(Node< T >* node, const BidirList< T >* list) :
    ptr(node),
    owner(list)
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

  template< class T >
  T& ListIter< T >::operator*() const
  {
    return ptr->_val;
  }

  template< class T >
  const T& ListConstIter< T >::operator*() const
  {
    return ptr->_val;
  }

  template< class T >
  T* ListIter< T >:: operator->() const
  {
    return &ptr->_val;
  }

  template< class T >
  const T* ListConstIter< T >:: operator->() const
  {
    return &ptr->_val;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator++()
  {
    ptr = ptr->_next;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator++()
  {
    ptr = ptr->_next;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator++(int)
  {
    ListIter< T > copy(*this);
    ptr = ptr->_next;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator++(int)
  {
    ListConstIter< T > copy(*this);
    ptr = ptr->_next;
    return copy;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator--()
  {
    if (ptr == nullptr)
    {
      ptr = owner->_tail;
      return *this;
    }
    ptr = ptr->_prev;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator--()
  {
    if (ptr == nullptr)
    {
      ptr = owner->_tail;
      return *this;
    }
    ptr = ptr->_prev;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator--(int)
  {
    if (ptr == nullptr)
    {
      ptr = owner->_tail;
      return *this;
    }
    ListIter< T > copy(*this);
    ptr = ptr->_prev;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator--(int)
  {
    if (ptr == nullptr)
    {
      ptr = owner->_tail;
      return *this;
    }
    ListConstIter< T > copy(*this);
    ptr = ptr->_prev;
    return copy;
  }

  template< class T >
  void BidirList< T >::push_front(const T& val)
  {
    if (_size)
    {
      Node< T >* elem = new Node< T >{val, _head, nullptr};
      _head->_prev = elem;
      _head = elem;
    }
    else
    {
      Node< T >* elem = new Node< T >{val, nullptr, nullptr};
      _head = elem;
      _tail = elem;
    }
    _size++;
  }

  template< class T >
  bool BidirList< T >::empty()
  {
    return !_size;
  }
}

#endif

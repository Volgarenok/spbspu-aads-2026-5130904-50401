#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <utility>
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
    Node< T >* _ptr;
    const BidirList< T >* _owner;
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
    Node< T >* _ptr;
    const BidirList< T >* _owner;
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
    BidirList(const BidirList< T >& list);
    BidirList(BidirList< T >&& list);
    BidirList< T >& operator=(const BidirList< T >& list);
    BidirList< T >& operator=(BidirList< T >&& list);
    size_t size() const;
    ListIter< T > begin();
    ListConstIter< T > begin() const;
    ListConstIter< T > cbegin() const;
    ListIter< T > end();
    ListConstIter< T > end() const;
    ListConstIter< T > cend() const;
    void push_back(const T& val);
    void push_front(const T& val);
    void push_back(T&& val);
    void push_front(T&& val);
    bool empty();
    T& front();
    T& back();
    ListIter< T > erase(ListIter< T > it);
    ListIter< T > erase(ListIter< T > start, ListIter< T > end);
    void pop_front();
    void pop_back();
    ListIter< T > insert(ListIter< T > it, const T& val);
    ListIter< T > insert(ListIter< T > it, T&& val);
    void clear();
    void swap(BidirList< T >& list1, BidirList< T >& list2);
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
    clear();
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
  ListConstIter< T > BidirList< T >::begin() const
  {
    return ListConstIter< T >(_head, this);
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
  ListConstIter< T > BidirList< T >::end() const
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cend() const
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListIter< T >::ListIter() :
    _ptr(nullptr),
    _owner(nullptr)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter() :
    _ptr(nullptr),
    _owner(nullptr)
  {}

  template< class T >
  ListIter< T >::ListIter(Node< T >* node, const BidirList< T >* list) :
    _ptr(node),
    _owner(list)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(Node< T >* node, const BidirList< T >* list) :
    _ptr(node),
    _owner(list)
  {}

  template< class T >
  bool ListIter< T >::operator==(const ListIter< T >& it) const
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListIter< T >::operator!=(const ListIter< T >& it) const
  {
    return !(_ptr == it._ptr);
  }

  template< class T >
  bool ListConstIter< T >::operator==(const ListConstIter< T >& it) const
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListConstIter< T >::operator!=(const ListConstIter< T >& it) const
  {
    return !(_ptr == it._ptr);
  }

  template< class T >
  void BidirList< T >::push_back(const T& val)
  {
    insert(end(), val);
  }

    template< class T >
  void BidirList< T >::push_back(T&& val)
  {
    insert(end(), std::move(val));
  }

  template< class T >
  void BidirList< T >::push_front(const T& val)
  {
    insert(begin(), val);
  }

  template< class T >
  void BidirList< T >::push_front(T&& val)
  {
    insert(begin(), std::move(val));
  }

  template< class T >
  T& ListIter< T >::operator*() const
  {
    return _ptr->_val;
  }

  template< class T >
  const T& ListConstIter< T >::operator*() const
  {
    return _ptr->_val;
  }

  template< class T >
  T* ListIter< T >:: operator->() const
  {
    return &_ptr->_val;
  }

  template< class T >
  const T* ListConstIter< T >:: operator->() const
  {
    return &_ptr->_val;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator++()
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator++()
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator++(int)
  {
    ListIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator++(int)
  {
    ListConstIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator--()
  {
    if (_ptr == nullptr)
    {
      _ptr = _owner->_tail;
      return *this;
    }
    _ptr = _ptr->_prev;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator--()
  {
    if (_ptr == nullptr)
    {
      _ptr = _owner->_tail;
      return *this;
    }
    _ptr = _ptr->_prev;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator--(int)
  {
    if (_ptr == nullptr)
    {
      _ptr = _owner->_tail;
      return *this;
    }
    ListIter< T > copy(*this);
    _ptr = _ptr->_prev;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator--(int)
  {
    if (_ptr == nullptr)
    {
      _ptr = _owner->_tail;
      return *this;
    }
    ListConstIter< T > copy(*this);
    _ptr = _ptr->_prev;
    return copy;
  }

  template< class T >
  bool BidirList< T >::empty()
  {
    return !_size;
  }

  template< class T >
  T& BidirList< T >::front()
  {
    return _head->_val;
  }
  template< class T >
  T& BidirList< T >::back()
  {
    return _tail->_val;
  }

  template< class T >
  ListIter< T > BidirList< T >::erase(ListIter< T > it)
  {
    Node< T >* next = it._ptr->_next;
    Node< T >* prev = it._ptr->_prev;
    delete it._ptr;
    if (next != nullptr)
    {
      next->_prev = prev;
    }
    else
    {
      _tail = prev;
    }

    if (prev != nullptr)
    {
      prev->_next = next;
    }
    else
    {
      _head = next;
    }
    it._ptr = next;
    _size--;
    return it;
  }

  template< class T >
  ListIter< T > BidirList< T >::erase(ListIter< T > first, ListIter< T > last)
  {
    while(first != last)
    {
      first = erase(first);
    }
    return first;
  }

  template< class T >
  void BidirList< T >::pop_front()
  {
    erase(begin());
  }

  template< class T >
  void BidirList< T >::pop_back()
  {
    erase(--end());
  }

  template< class T >
  ListIter< T > BidirList< T >::insert(ListIter< T > it, const T& val)
  {
    Node< T >* new_node;
    if (empty())
    {
      new_node = new Node< T >{val, nullptr, nullptr};
      _head = new_node;
      _tail = new_node;
    }
    else if (it._ptr == nullptr)
    {
      new_node = new Node< T >{val, nullptr, _tail};
      _tail->_next = new_node;
      _tail = new_node;
    }
    else
    {
      new_node = new Node< T >{val, it._ptr, it._ptr->_prev};
      if (it._ptr->_prev == nullptr)
      {
        _head->_prev = new_node;
        _head = new_node;
      }
      else
      {
        it._ptr->_prev->_next = new_node;
        it._ptr->_prev = new_node;
      }
    }
    _size++;
    return ListIter< T >(new_node, this);
  }

  template< class T >
  ListIter< T > BidirList< T >::insert(ListIter< T > it, T&& val)
  {
    Node< T >* new_node;
    if (empty())
    {
      new_node = new Node< T >{std::move(val), nullptr, nullptr};
      _head = new_node;
      _tail = new_node;
    }
    else if (it._ptr == nullptr)
    {
      new_node = new Node< T >{std::move(val), nullptr, _tail};
      _tail->_next = new_node;
      _tail = new_node;
    }
    else
    {
      new_node = new Node< T >{std::move(val), it._ptr, it._ptr->_prev};
      if (it._ptr->_prev == nullptr)
      {
        _head->_prev = new_node;
        _head = new_node;
      }
      else
      {
        it._ptr->_prev->_next = new_node;
        it._ptr->_prev = new_node;
      }
    }
    _size++;
    return ListIter< T >(new_node, this);
  }

  template< class T >
  void BidirList< T >::clear()
  {
    erase(begin(), end());
  }

  template< class T >
  BidirList< T >::BidirList(const BidirList< T >& list) :
    _head(nullptr),
    _tail(nullptr),
    _size(0)
  {
    try
    {
      for(ListConstIter< T > it = list.begin(); it != list.end(); ++it)
      {
        push_back(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  BidirList< T >::BidirList(BidirList< T >&& list) :
    _head(list._head),
    _tail(list._tail),
    _size(list._size)
  {
    list._head = nullptr;
    list._tail = nullptr;
    list._size = 0;
  }

  template< class T >
  void BidirList< T >::swap(BidirList< T >& list1, BidirList< T >& list2)
  {
    std::swap(list1._head, list2._head);
    std::swap(list1._tail, list2._tail);
    std::swap(list1._size, list2._size);
  }

  template< class T >
  BidirList< T >& BidirList< T >::operator=(const BidirList< T >& list)
  {
    BidirList< T > temp(list);
    swap(*this, temp);
    return *this;
  }

  template< class T >
  BidirList< T >& BidirList< T >::operator=(BidirList< T >&& list)
  {
    if (this == std::addressof(list))
    {
      return *this;
    }
    clear();
    _head = list._head;
    _tail = list._tail;
    _size = list._size;
    list._head = nullptr;
    list._tail = nullptr;
    list._size = 0;
    return *this;
  }
}

#endif

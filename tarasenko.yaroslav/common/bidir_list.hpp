#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <utility>
#include <cstddef>
#include <memory>

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
    bool operator==(const ListIter< T >& it) const noexcept;
    bool operator!=(const ListIter< T >& it) const noexcept;
    T& operator*() const;
    T* operator->() const;
    ListIter< T >& operator++() noexcept;
    ListIter< T > operator++(int) noexcept;
    ListIter< T >& operator--() noexcept;
    ListIter< T > operator--(int) noexcept;
  private:
    ListIter(Node< T >* node, const BidirList< T >* list) noexcept;
  };

  template< class T >
  class ListConstIter
  {
    friend class BidirList< T >;
    Node< T >* _ptr;
    const BidirList< T >* _owner;
  public:
    ListConstIter();
    bool operator==(const ListConstIter< T >& it) const noexcept;
    bool operator!=(const ListConstIter< T >& it) const noexcept;
    const T& operator*() const;
    const T* operator->() const;
    ListConstIter< T >& operator++() noexcept;
    ListConstIter< T > operator++(int) noexcept;
    ListConstIter< T >& operator--() noexcept;
    ListConstIter< T > operator--(int) noexcept;
  private:
    ListConstIter(Node< T >* node, const BidirList< T >* list) noexcept;
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
    BidirList(BidirList< T >&& list) noexcept;
    BidirList< T >& operator=(const BidirList< T >& list);
    BidirList< T >& operator=(BidirList< T >&& list) noexcept;
    size_t size() const noexcept;
    ListIter< T > begin() noexcept;
    ListConstIter< T > begin() const noexcept;
    ListConstIter< T > cbegin() const noexcept;
    ListIter< T > end() noexcept;
    ListConstIter< T > end() const noexcept;
    ListConstIter< T > cend() const noexcept;
    void push_back(const T& val);
    void push_front(const T& val);
    void push_back(T&& val);
    void push_front(T&& val);
    bool empty() const noexcept;
    T& front() const;
    T& back() const;
    ListIter< T > erase(ListIter< T > it) noexcept;
    ListIter< T > erase(ListIter< T > start, ListIter< T > end) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;
    ListIter< T > insert(ListIter< T > it, const T& val);
    ListIter< T > insert(ListIter< T > it, T&& val);
    void clear() noexcept;
    void swap(BidirList< T >& list1, BidirList< T >& list2) noexcept;
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
  size_t BidirList< T >::size() const noexcept
  {
    return _size;
  }

  template< class T >
  ListIter< T > BidirList< T >::begin() noexcept
  {
    return ListIter< T >(_head, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::begin() const noexcept
  {
    return ListConstIter< T >(_head, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cbegin() const noexcept
  {
    return ListConstIter< T >(_head, this);
  }

   template< class T >
  ListIter< T > BidirList< T >::end() noexcept
  {
    return ListIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::end() const noexcept
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cend() const noexcept
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
  ListIter< T >::ListIter(Node< T >* node, const BidirList< T >* list) noexcept :
    _ptr(node),
    _owner(list)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(Node< T >* node, const BidirList< T >* list) noexcept :
    _ptr(node),
    _owner(list)
  {}

  template< class T >
  bool ListIter< T >::operator==(const ListIter< T >& it) const noexcept
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListIter< T >::operator!=(const ListIter< T >& it) const noexcept
  {
    return !(_ptr == it._ptr);
  }

  template< class T >
  bool ListConstIter< T >::operator==(const ListConstIter< T >& it) const noexcept
  {
    return _ptr == it._ptr;
  }

  template< class T >
  bool ListConstIter< T >::operator!=(const ListConstIter< T >& it) const noexcept
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
  ListIter< T >& ListIter< T >::operator++() noexcept
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator++() noexcept
  {
    _ptr = _ptr->_next;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator++(int) noexcept
  {
    ListIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator++(int) noexcept
  {
    ListConstIter< T > copy(*this);
    _ptr = _ptr->_next;
    return copy;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator--() noexcept
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
  ListConstIter< T >& ListConstIter< T >::operator--() noexcept
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
  ListIter< T > ListIter< T >::operator--(int) noexcept
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
  ListConstIter< T > ListConstIter< T >::operator--(int) noexcept
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
  bool BidirList< T >::empty() const noexcept
  {
    return !_size;
  }

  template< class T >
  T& BidirList< T >::front() const
  {
    return _head->_val;
  }
  template< class T >
  T& BidirList< T >::back() const
  {
    return _tail->_val;
  }

  template< class T >
  ListIter< T > BidirList< T >::erase(ListIter< T > it) noexcept
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
  ListIter< T > BidirList< T >::erase(ListIter< T > first, ListIter< T > last) noexcept
  {
    while(first != last)
    {
      first = erase(first);
    }
    return first;
  }

  template< class T >
  void BidirList< T >::pop_front() noexcept
  {
    erase(begin());
  }

  template< class T >
  void BidirList< T >::pop_back() noexcept
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
  void BidirList< T >::clear() noexcept
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
  BidirList< T >::BidirList(BidirList< T >&& list) noexcept :
    _head(list._head),
    _tail(list._tail),
    _size(list._size)
  {
    list._head = nullptr;
    list._tail = nullptr;
    list._size = 0;
  }

  template< class T >
  void BidirList< T >::swap(BidirList< T >& list1, BidirList< T >& list2) noexcept
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
  BidirList< T >& BidirList< T >::operator=(BidirList< T >&& list) noexcept
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

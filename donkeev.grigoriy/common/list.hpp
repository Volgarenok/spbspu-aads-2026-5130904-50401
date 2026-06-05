#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include "node.hpp"
#include "iterator.hpp"
#include <iostream>

namespace donkeev
{
  template< class T > class LIter;
  template< class T > class Node;

  template< class T >
  class List
  {
    Node< T >* head_;
    Node< T >* tail_;
    size_t length_;

  public:
    List();
    List(size_t, T);
    List(const List< T >&);
    List(List< T >&&) noexcept;

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List&&) noexcept;

    ~List();

    LIter< T > begin();
    LCIter< T > cbegin() const;

    LIter< T > pushAfter(LIter< T >, const T&);
    void pushFront(const T&);
    void pushBack(const T&);

    void popFront();
    void cutAfter(LIter< T >&);
    void clearAll();
    void deleteNode(const T&);
    bool has(const T&);

    bool isEmpty() const;
    size_t size() const;

    void swap(List< T >&) noexcept;
  };

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    length_(0)
  {}
  template< class T >
  List< T >::List(size_t size, T data):
    head_(nullptr),
    tail_(nullptr),
    length_(size)
  {
    if (length_ == 0)
    {
      return;
    }
    head_ = new Node< T >{data, nullptr};
    head_->next = head_;
    tail_ = head_;
    Node< T >* tmp = head_;
    for (size_t i = 1; i < length_; ++i)
    {
      tmp->next = new Node< T >{data, nullptr};
      tmp = tmp->next;
    }
    tail_ = tmp;
    tail_->next = head_;
  }
  template< class T >
  List< T >::List(const List< T >& yaList):
    head_(nullptr),
    tail_(nullptr),
    length_(0)
  {
    if (yaList.size() == 0)
    {
      return;
    }
    LCIter< T > it{yaList.head_};
    for (size_t i = 0; i < yaList.size(); ++i)
    {
      pushBack(*it);
      ++it;
    }
  }
  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(other.head_),
    tail_(other.tail_),
    length_(other.length_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.length_ = 0;
  }

  template< class T >
  donkeev::List< T >& List< T >::operator=(const List< T >& other)
  {
    List< T > cpy{other};
    swap(cpy);
    return *this;
  }
  template< class T >
  donkeev::List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    List< T > cpy{std::move(other)};
    swap(cpy);
    return *this;
  }

  template< class T >
  donkeev::List< T >::~List()
  {
    clearAll();
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >{head_};
  }
  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >{head_};
  }

  template< class T >
  LIter< T > List< T >::pushAfter(LIter< T > it, const T& value)
  {
    Node< T >* tmp = new Node< T >{value, it.n->next};
    it.n->next = tmp;
    ++length_;
    return {tmp};
  }
  template< class T >
  void List< T >::pushFront(const T& value)
  {
    Node < T >* tmp = new Node< T >{value, head_};
    if (length_ == 0)
    {
      tail_ = tmp;
    }
    head_ = tmp;
    tail_->next = head_;
    ++length_;
  }
  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* tmp = new Node< T >{value, head_};
    if (length_ == 0)
    {
      head_ = tmp;
      tail_ = tmp;
      tail_->next = tmp;
      head_->next = tmp;
      ++length_;
      return;
    }
    tail_->next = tmp;
    tail_ = tmp;
    ++length_;
  }

  template< class T >
  void List< T >::popFront()
  {
    if (length_ == 1)
    {
      head_->next = nullptr;
      tail_->next = nullptr;
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      --length_;
      return;
    }
    Node< T >* tmp = head_->next;
    head_->next = nullptr;
    delete head_;
    head_ = tmp;
    head_->next = tmp->next;
    tail_->next = head_;
    --length_;
  }
  template< class T >
  void List< T >::cutAfter(LIter< T >& it)
  {
    Node< T >* deleteNode = it.n->next;
    if (length_ == 1)
    {
      delete it.n;
      it.n = nullptr;
      head_ = nullptr;
      tail_ = nullptr;
      --length_;
      return;
    }
    else if (deleteNode == tail_)
    {
      tail_ = it.n;
    }
    else if (deleteNode == head_)
    {
      popFront();
      return;
    }
    it.n->next = deleteNode->next;
    delete deleteNode;
    --length_;
  }
  template< class T >
  void List< T >::clearAll()
  {
    while (length_)
    {
      popFront();
    }
    head_ = nullptr;
    tail_ = nullptr;
  }
  template< class T >
  void List< T >::deleteNode(const T& value)
  {
    Node< T >* node = head_;
    if (node->val == value)
    {
      head_ = head_->next;
      tail_ = head_;
      --length_;
      return;
    }
    donkeev::LIter< T > it = begin();
    for (size_t i = 0; i < length_; ++i)
    {
      if (*it == value)
      {
        cutAfter(it);
      }
      ++it;
    }
  }
  template< class T >
  bool List< T >::has(const T& value)
  {
    Node< T >* it = head_;
    for (size_t i = 0; i < length_; ++i)
    {
      if (it->val == value)
      {
        return true;
      }

      ++it;
    }
    return false;
  }

  template< class T >
  bool List< T >::isEmpty() const
  {
    return (length_ == 0);
  }
  template< class T >
  size_t List< T >::size() const
  {
    return length_;
  }

  template< class T >
  void donkeev::List< T >::swap(List< T >& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(length_, other.length_);
  }
}
#endif

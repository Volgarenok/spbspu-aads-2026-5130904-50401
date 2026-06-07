#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include "node.hpp"
#include "iterator.hpp"

namespace donkeev
{
  using detail::Node;

  template< class T > class LIter;

  template< class T >
  class List
  {
  public:
    List() noexcept;
    List(size_t, const T&);
    List(const List< T >&);
    List(List< T >&&) noexcept;

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List&&) noexcept;

    ~List() noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > insertAfter(LIter< T >, const T&);
    void pushFront(const T&);
    void pushFront(T&&);
    void pushBack(const T&);
    void pushBack(T&&);

    void popFront() noexcept;
    void eraseAfter(LIter< T >&) noexcept;
    void clear() noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t length_;
  };

  template< class T >
  List< T >::List() noexcept:
    head_(nullptr),
    tail_(nullptr),
    length_(0)
  {}
  template< class T >
  List< T >::List(size_t size, const T& data):
    head_(nullptr),
    tail_(nullptr),
    length_(size)
  {
    if (length_ == 0)
    {
      return;
    }

    size_t created = 0;
    try
    {
      head_ = new Node< T >{data, nullptr};
      ++created;

      head_->next = head_;
      tail_ = head_;
      Node< T >* tmp = head_;
      for (; created < length_; ++created)
      {
        tmp->next = new Node< T >{data, nullptr};
        tmp = tmp->next;
      }
      tail_ = tmp;
      tail_->next = head_;
    }
    catch (...)
    {
      if (created == 1)
      {
        delete head_;
      }
      else
      {
        Node<T>* current = head_;
        for (size_t i = 0; i < created; ++i)
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
      }

      head_ = nullptr;
      tail_ = nullptr;
      length_ = 0;
      throw;
    }
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
    size_t i = 0;
    try
    {
      for (; i < yaList.size(); ++i)
      {
        pushBack(*it);
        ++it;
      }
    }
    catch (...)
    {
      clear();
      throw;
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
    clear();
    LCIter<T> it = other.cbegin();
    for (size_t i = 0; i < other.size(); ++i)
    {
      pushBack(*it);
      ++it;
    }

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
  donkeev::List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >{head_};
  }
  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >{head_};
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > it, const T& value)
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
      head_ = tmp;
      tail_ = tmp;
      tail_->next = head_;
      head_->next = tail_;
      ++length_;
      return;
    }
    head_ = tmp;
    tail_->next = head_;
    ++length_;
  }
  template< class T >
  void List< T >::pushFront(T&& value)
  {
    Node < T >* tmp = new Node< T >{std::move(value), head_};
    if (length_ == 0)
    {
      head_ = tmp;
      tail_ = tmp;
      tail_->next = head_;
      head_->next = tail_;
      ++length_;
      return;
    }
    tmp->next = head_;
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
  void List< T >::pushBack(T&& value)
  {
    Node< T >* tmp = new Node< T >{std::move(value), head_};
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
  void List< T >::popFront() noexcept
  {
    Node< T >* tmp = head_->next;
    delete head_;
    head_ = tmp;
    tail_->next = head_;
    --length_;
  }
  template< class T >
  void List< T >::eraseAfter(LIter< T >& it) noexcept
  {
    Node< T >* deleteNode = it.n->next;
    if (length_ == 1)
    {
      delete it.n;
      it.n = nullptr;
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
  void List< T >::clear() noexcept
  {
    LIter< T > it{head_};
    while (length_)
    {
      eraseAfter(it);
    }

    head_ = nullptr;
    tail_ = nullptr;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return (length_ == 0);
  }
  template< class T >
  size_t List< T >::size() const noexcept
  {
    return length_;
  }
}
#endif

#ifndef LIST_HPP
#define LIST_HPP

#include "iter.hpp"
#include "const-iter.hpp"

#include <cstddef>

namespace lukashevich
{
  template< class T >
  class Node
  {
    public:
      Node(T value, Node<T>* n, Node<T>* p):
        val(value),
        next(n),
        prev(p)
      {}

      T val;
      Node< T >* next;
      Node< T >* prev;
  };

  template< class T >
  class LIter;

  template< class T >
  class LCIter;

  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T >& list);
      List< T >& operator=(const List< T >& list);
      List(List< T >&& list);
      List< T >& operator=(List< T >&& list);

      T& front();
      T& back();
      const T& front() const;
      const T& back() const;

      void pushFront(const T& value);
      void pushBack(const T& value);

      void popFront();
      void popBack();

      void clear();

      size_t size() const;

      LIter< T > begin();
      LIter< T > end();
      LCIter< T > begin() const;
      LCIter< T > end() const;

      size_t size() const;
      bool empty() const;

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

  template< class T >
  List< T >::List():
    fake_(makeFake< T >()),
    size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
    removeFake(fake_);
  }

  template< class T >
  void List< T >::clear()
  {
    Node< T >* cur = fake_->next;
    while (cur != nullptr) {
      Node< T >* next = cur->next;
      delete cur;
      cur = next;
    }
    fake_->next = nullptr;
    fake_->prev = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    Node< T >* node = new Node< T >(value, fake_->next, fake_);

    if (fake_->next != nullptr) {
      fake_->next->prev = node;
    }
    else {
      fake_->prev = node;
    }
    fake_->next = node;
    size_++;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* node = new Node< T >(value, nullptr, fake_->prev);

    if (fake_->prev != nullptr) {
      fake_->prev->next = node;
    }
    else {
      fake_->next = node;
    }

    fake_->prev = node;
    size_++;
  }

  template< class T >
  const T& List< T >::front() const
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  T& List< T >::front()
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  const T& List< T >::back() const
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }

  template< class T >
  T& List< T >::back()
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }

  template< class T >
  void List< T >::popFront()
  {
    assert(fake_->next != nullptr);

    Node< T >* node = fake_->next;
    fake_->next = node->next;

    if (fake_->next != nullptr) {
      fake_->next->prev = fake_;
    }
    else {
      fake_->prev = nullptr;
    }

    delete node;
    size_--;
  }

  template< class T >
  void List< T >::popBack()
  {
    assert(fake_->prev != nullptr);

    Node< T >* node = fake_->prev;
    fake_->prev = node->prev;

    if (fake_->prev != fake_) {
      fake_->prev->next = nullptr;
    }
    else {
      fake_->prev = nullptr;
      fake_->next = nullptr;
    }

    delete node;
    size_--;
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::begin() const
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template< class T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  List< T >::List(const List< T >& list):
    fake_(makeFake< T >()),
    size_(0)
  {
    Node< T >* cur = list.fake_->next;

    while (cur != nullptr) {
      pushBack(cur->val);
      cur = cur->next;
    }
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& list)
  {
    if (this == &list) {
      return *this;
    }
    clear();

    Node< T >* cur = list.fake_->next;
    while (cur != nullptr) {
      pushBack(cur->val);
      cur = cur->next;
    }
    return *this;
  }

  template< class T >
  List< T >::List(List< T >&& list):
    fake_(list.fake_),
    size_(list.size_)
  {
    list.fake_ = makeFake< T >();
    list.size_ = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& list)
  {
    if (this == &list) {
      return *this;
    }

    clear();
    removeFake(fake_);

    fake_ = list.fake_;
    size_ = list.size_;

    list.fake_ = makeFake< T >();
    list.size_ = 0;

    return *this;
  }
}
#endif

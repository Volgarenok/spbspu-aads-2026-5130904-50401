#ifndef LIST
#define LIST
#include "iterator.hpp"
#include "const-iterator.hpp"
#include "node.hpp"
#include <iostream>
#include <cstddef>
#include <utility>
#include <string>

namespace malashenko {
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T >
  class List {
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other);

    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other);

    LIter< T > begin() const;
    LIter< T > end() const;
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;

    T& front() const;
    T& back() const;
    LIter< T > getFake() const;

    LIter< T > insert(LIter< T > h, const T& value);
    void push_back(const T& value);
    void push_front(const T& value);

    LIter< T > cut(LIter< T > h);
    LIter< T > erase(LIter< T > h);
    void pop_back();
    void pop_front();


    void clear();
    size_t size() const;
    void swap(List< T >& other);
    ~List();
    bool empty() const;
  private:
    void rmFake();
    Node< T >* fake_;
    size_t s_;
  };

  template< class T >
  void List< T >::rmFake()
  {
    ::operator delete(fake_);
  }
  template< class T >
  List< T >::List():
    s_(0)
  {
    fake_ = static_cast< Node< T >* >(::operator new (sizeof(Node< T >)));
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template< class T >
  List< T >::List(const List< T >& other):
    s_(0)
  {
    fake_ = static_cast< Node< T >* >(::operator new (sizeof(Node< T >)));
    fake_->next = fake_;
    fake_->prev = fake_;
    for (LIter< T > start = other.begin(); start != other.end(); ++start)
    {
      try
      {
        push_back(*start);
      }
      catch(...)
      {
        clear();
        throw;
      }
    }
  }

  template< class T >
  List< T >::List(List< T >&& other):
    fake_(std::move(other.fake_)),
    s_(std::move(other.s_))
  {}

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    fake_ = std::move(other.fake_);
    s_ = std::move(other.s_);
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    List< T > temp(other);
    swap(temp);
    return *this;
  }

  template< class T >
  void List< T >::swap(List< T >& other)
  {
    using std::swap;
    swap(fake_, other.fake_);
    swap(s_, other.s_);
  }

  template< class T >
  LIter< T > List< T >::begin() const
  {
    return LIter< T >(fake_->next);
  }

  template< class T >
  LIter< T > List< T >::end() const
  {
    return LIter< T >(fake_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::cend() const
  {
    return LCIter< T >(fake_);
  }

  template< class T >
  T& List< T >::front() const
  {
    return fake_->next->value_;
  }

  template< class T >
  T& List< T >::back() const
  {
    return fake_->prev->value_;
  }

  template< class T >
  LIter< T > List< T >::getFake() const
  {
    return LIter< T >(fake_);
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > h, const T& value)
  {
    Node< T >* newNode = new Node< T >{value, h.node_->next, h.node_};
    h.node_->next = newNode;
    newNode->next->prev = newNode;
    ++s_;
    return {newNode};
  }

  template< class T >
  void List< T >::push_back(const T& value)
  {
    LIter< T > itBack = end().node_->prev;
    insert(itBack, value);
  }

  template< class T >
  void List< T >::push_front(const T& value)
  {
    insert(fake_, value);
  }

  template< class T >
  LIter< T > List< T >::cut(LIter< T > h)
  {
    if (empty())
    {
      return end();
    }
    LIter< T > ret = h + 1;
    h.node_->prev->next = ret.node_;
    ret.node_->prev = h.node_->prev;
    delete h.node_;
    s_--;
    return ret;
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > h)
  {
    if (s_ == 1)
    {
      cut(h);
    }
    return cut(h + 1);
  }

  template< class T >
  void List< T >::pop_back()
  {
    cut(fake_->prev);
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (s_ == 1)
    {
      pop_back();
    }
    erase(fake_);
  }

  template< class T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_back();
    }
    s_ = 0;
  }

  template< class T >
  size_t List< T >::size() const
  {
    return s_;
  }

  template< class T >
  bool List< T >::empty() const
  {
    return s_ == 0;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
    rmFake();
  }
};

namespace std {
  template< class T >
  void swap(malashenko::List< T >& a, malashenko::List< T >& b)
  {
    a.swap(b);
  }
}

#endif

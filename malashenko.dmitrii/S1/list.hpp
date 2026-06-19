#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <cstddef>
#include <string>
#include <utility>
#include "iterator.hpp"
#include "const_iterator.hpp"
#include "node.hpp"

namespace malashenko {
  template< class T > class LIter;
  template< class T > class LCIter;
  template< class T >
  class List {
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;

    ~List() noexcept;

    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other) noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;

    T& front();
    T& back();

    LIter< T > insert(LIter< T > h, const T& value);
    LIter< T > insert(LIter< T > h, T&& value);
    void push_back(const T& value);
    void push_back(T&& value);

    void push_front(const T& value);
    void push_front(T&& value);

    LIter< T > cut(LIter< T > h);
    LIter< T > erase(LIter< T > h);
    void pop_back();
    void pop_front();


    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List< T >& other) noexcept;
    bool empty() const noexcept;
  private:
    detail::Node< T >* fake_;
    size_t s_;
    void rmFake();
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
    fake_ = static_cast< detail::Node< T >* >(::operator new (sizeof(detail::Node< T >)));
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template< class T >
  List< T >::List(const List< T >& other):
    fake_(reinterpret_cast< detail::Node< T >* >(::operator new (sizeof(detail::Node< T >)))),
    s_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
    for (LCIter< T > start = other.begin(); start != other.end(); ++start)
    {
      try
      {
        push_back(*start);
      }
      catch (...)
      {
        clear();
        rmFake();
        throw;
      }
    }
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    fake_(std::exchange(other.fake_, nullptr)),
    s_(std::exchange(other.s_, 0))
  {}

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    assert(this != &other);

    List< T > temp(std::move(other));
    swap(temp);
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
  void List< T >::swap(List< T >& other) noexcept
  {
    using std::swap;
    swap(fake_, other.fake_);
    swap(s_, other.s_);
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake_->next);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(fake_);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(fake_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(fake_);
  }

  template< class T >
  T& List< T >::front()
  {
    return fake_->next->value_;
  }

  template< class T >
  T& List< T >::back()
  {
    return fake_->prev->value_;
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > h, const T& value)
  {
    detail::Node< T >* newNode = new detail::Node< T >{value, h.node_->next, h.node_};
    h.node_->next = newNode;
    newNode->next->prev = newNode;
    ++s_;
    return {newNode};
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > h, T&& value)
  {
    detail::Node< T >* newNode = new detail::Node< T >{std::forward< T >(value), h.node_->next, h.node_};
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
  void List< T >::push_back(T&& value)
  {
    LIter< T > itBack = end().node_->prev;
    insert(itBack, std::forward< T >(value));
  }

  template< class T >
  void List< T >::push_front(const T& value)
  {
    insert(fake_, value);
  }

  template< class T >
  void List< T >::push_front(T&& value)
  {
    insert(fake_, std::forward< T >(value));
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
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_back();
    }
    s_ = 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return s_;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return s_ == 0;
  }

  template< class T >
  List< T >::~List() noexcept
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

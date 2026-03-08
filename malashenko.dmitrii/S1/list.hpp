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

    void push_back(const T& value);
    void push_front(const T& value);

    void pop_back();
    void pop_front();

    void clear();
    size_t size() const;
    void swap(List< T >& other);
    ~List();
  private:
    Node< T >* head_;
    Node< T >* tail_;
    Node< T >* fake_;
    size_t s_;
  };

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    s_(0)
  {
    fake_ = static_cast< Node< T >* >(::operator new (sizeof(Node< T >)));
    fake_->next = nullptr;
  }

  template< class T >
  List< T >::List(const List< T >& other):
    head_(nullptr),
    tail_(nullptr),
    s_(0)
  {
    fake_ = static_cast< Node< T >* >(::operator new (sizeof(Node< T >)));
    try
    {
      if (!(other.head_))
      {
        return;
      }

      for (LCIter< T > start = other.cbegin(); start != other.cend(); ++start)
      {
        push_back(*start);
      }

      push_back(other.back());
    } catch(...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List< T >&& other):
    fake_(std::move(other.fake_)),
    head_(std::move(other.head_)),
    tail_(std::move(other.tail_)),
    s_(std::move(other.s_))
  {}

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this = &other)
    {
      return *this;
    }
    clear();
    fake_ = std::move(other.fake_);
    head_ = std::move(other.head_);
    tail_ = std::move(other.tail_);
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
    swap(tail_, other.tail_);
    swap(head_, other.head_);
  }

  template< class T >
  LIter< T > List< T >::begin() const
  {
    assert(head_ != nullptr && "List is empty");
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end() const
  {
    assert(tail_ != nullptr && "List is empty");
    return LIter< T >(tail_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    assert(head_ != nullptr && "List is empty");
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const
  {
    assert(tail_ != nullptr && "List is empty");
    return LCIter< T >(tail_);
  }

  template< class T >
  T& List< T >::front() const
  {
    assert(head_ != nullptr && "List is empty");
    return head_->value_;
  }

  template< class T >
  T& List< T >::back() const
  {
    assert(tail_ != nullptr && "List is empty");
    return tail_->value_;
  }

  template< class T >
  LIter< T > List< T >::getFake() const
  {
    return LIter< T >(fake_);
  }

  template< class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >{value, head_, tail_};
    if (!tail_)
    {
      head_ = newNode;
      tail_ = newNode;
      fake_->next = head_;
    } else
    {
      tail_->next = newNode;
      head_->prev = newNode;
      tail_ = tail_->next;
    }
    s_++;
  }

  template< class T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >{value, head_, tail_};
    if (!head_)
    {
      head_ = newNode;
      tail_ = newNode;
      fake_->next = head_;
    } else
    {
      tail_->next = newNode;
      head_->prev = newNode;
      head_ = head_->prev;
      fake_->next = head_;
    }
    s_++;
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      fake_->next = nullptr;
      s_ = 0;
      return;
    }

    Node< T >* tmpNode = tail_->prev;

    head_->prev = tmpNode;
    tmpNode->next = head_;

    delete tail_;
    tail_ = tmpNode;
    s_--;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      fake_->next = nullptr;
      s_ = 0;
      return;
    }

    Node< T >* tmpNode = head_->next;

    tail_->next = tmpNode;
    tmpNode->prev = tail_;
    fake_->next = tmpNode;

    delete head_;
    tail_ = tmpNode;
    s_--;
  }

  template< class T >
  void List< T >::clear()
  {
    while (fake_->next != nullptr)
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
  List< T >::~List()
  {
    clear();
    ::operator delete(fake_);
  }
};

namespace std {
  template< class T >
  void swap(malashenko::List< T >& a, malashenko::List< T >& b)
  {
    a.swap(b);
  }
};

#endif

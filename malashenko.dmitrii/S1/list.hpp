#ifndef LIST
#define LIST
#include "iterator.hpp"
#include "node.hpp"
#include <iostream>
#include <cstddef>
#include <utility>
#include <string>


namespace malashenko {
  template< class T >
  class LIter;
  template< class T >
  class LCIter;

  template< class T >
  class List {
  public:
    List();

    List(const List< T >& other);
    List(List< T >&& other);

    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other);

    // LIter< T > begin();
    // LIter< T > end();

    T& front();
    T& back();

    void push_back(const T& value);
    void push_front(const T& value);

    void pop_back() noexcept;
    void pop_front() noexcept;

    void clear();
    void swap(const List< T >& other);
    ~List();
  private:
    Node< T >* head_;
    Node< T >* tail_;
    Node< T >* fake_;
  };
  
  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr)
  {
    fake_ = static_cast< Node< T >* >(::operator new (sizeof(List< T >*)));
    fake_->next = nullptr;
  }

  template< class T >
  List< T >::List(const List< T >& other)
  {
  }


  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this == &other)
    {
      return *this;
    }

    List< T > temp(other);
    std::swap(temp);

    return *this;
  }


  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    delete head_;
    delete tail_;

    head_ = std::move(other.head_);
    tail_ = std::move(other.tail_);

    return *this;
  }

  template< class T >
  void List< T >::swap(const List< T >& other)
  {
    using std::swap;
    swap(head_, other.head_);
    swap(tail_, other.tail_);
  }

  template< class T >
  T& List< T >::front()
  {
    if (!head_)
    {
      throw std::invalid_argument("List is empty");
    }
    return head_->value_;
  }


  template< class T >
  T& List< T >::back()
  {
    if (!tail_)
    {
      throw std::invalid_argument("List is empty");
    }
    return tail_->value_;
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
  }

  template< class T >
  void List< T >::pop_back() noexcept
  {
    if (!tail_)
    {
      return;
    }

    if(head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      fake_->next = nullptr;
      return;
    }

    Node< T >* tmpNode = tail_->prev;

    head_->prev = tmpNode;
    tmpNode->next = head_;

    delete tail_;
    tail_ = tmpNode;
  }

  template< class T >
  void List< T >::pop_front() noexcept
  {
    if(!head_)
    {
      return;
    }

    if(head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      fake_->next = nullptr;
      return;
    }

    Node< T >* tmpNode = head_->next;

    tail_->next = tmpNode;
    tmpNode->prev = tail_;
    fake_->next = tmpNode;

    delete tail_;
    tail_ = tmpNode;
  }

  template< class T >
  void List< T >::clear()
  {
    while (fake_->next != nullptr)
    {
      pop_back();
      // ::operator delete(fake_);
    }
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

};


#endif
#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace studilova
{
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
  };

  template< class T >
  class List
  {
    private:
      Node< T >* head_;
      size_t size_;
    public:
      List();
      ~List();

      bool empty() const;

      void clear();

      void pop_front();
      void pop_back();
  };

  template< class T >
  List< T >::List()
    : head_(nullptr), size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  void List< T >::clear()
  {
    while(!empty())
    {
      pop_front();
    }
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty())
    {
      return;
    }

    Node< T >* temp = head_;

    if (size_ == 1)
    {
      delete temp;
      head_ = nullptr;
      size_ = 0;
    } else {
      Node< T >* tail = head_->prev;

      head_ = head_->next;

      head_->prev = tail;
      tail->next = head_;

      delete temp;
      size_--;
    }
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (empty())
    {
      return;
    }

    if (size_ == 1)
    {
      delete head_;
      head_ = nullptr;
      size_ = 0;
    } else {
      Node< T >* tail = head_->prev;
      Node< T >* new_tail = tail->prev;

      new_tail->next = head_;
      head_->prev = new_tail;

      delete tail;
      size_--;
    }
  }

  template< class T >
  class LIter;

  template< class T >
  class CLIter;
}

#endif

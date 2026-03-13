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
  }

  template< class T >
  class LIter;

  template< class T >
  class CLIter;
}

#endif

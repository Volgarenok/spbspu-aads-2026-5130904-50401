#ifndef LIST
#define LIST
#include "iterator.hpp"
#include <iostream>

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

    LIter< T > begin();
    LIter< T > end();

    T& front();
    T& back();

    void push_back(const T& value);
    void push_front(const T& value);

    void pop_back();
    void pop_front();

    void clear();

    void swap(const List< T >& other);
    ~List();

  private:
  
    template< class T >
    class Node {
      T value_;
      Node< T >* next;
      Node< T >* prev;
    };

    Node< T >* head_;
    Node< T >* tail_;
    Node< T >* fake_;
  };
  
  template< class T >
  List< T >::List():
    head_(new Node{T(), nullptr, nullptr}),
    tail_(head_)
  {
    Node< T >* r = static_cast< Node< T >* >(::operator new (sizeof(Node< T >*)));
    r->next = head_;
    r->prev = nullptr;
    fake_ = r;
  }

  template< class T >
  List< T >::List(const List< T >& other):
  {
    Node< T >* fake = static_cast< Node< T >* >(::operator new (sizeof(Node< T >*)));
    fake_ = fake;
    Node< T >* head = new Node< T >{other.head_->value_, nullptr, fake};
    fake->next = head;
    while (other.head_ != other.tail_)
    {
      try {
        Node< T > tmp = new Node< T >{other.head_->value_, fake->next, head};
        head->next = tmp;
        head = head->next;
      }
      catch(...) {
        
        
      }
      
      
    }
    
    
  }

  template< class T >
  List< T >::List(List< T >&& other):
    head_(other.head_),
    tail_(other.tail_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
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
    return head_->value_;
  }


  template< class T >
  T& List< T >::back()
  {
    return tail_->value_;
  }

  template< class T >
  void List< T >::push_back(const T& value)
  {
    Node< T > newNode = new Node< T >{value, head_, tail_};
    tail_->next = newNode;
    tail_ = newNode;
  }



};


#endif
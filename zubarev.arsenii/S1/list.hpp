#ifndef LIST_HPP
#define LIST_HPP
#include "all.hpp"
namespace zubarev
{
  // template <class T>
  // class LIter;
  template < class T >
  class List
  {

  private:
    struct Node
    {
      T val;
      Node* next;

      Node(const T& v, Node* n = nullptr):
        val(v),
        next(n)
      {};

    };

    Node* head_;
    Node* ctFake() {
      Node* el=new Node<T>{T(),nullptr};
      return el;
    }
    void rmFake() noexcept
    {
      delete head_;
    }
  public:

    List();
    ~List();
    List(const List& list);
    List(List&& list) noexcept;

    LIter< T > begin();
    void clear();
    LIter< T > end();
    bool empty() const;
    void pop_front(const T&);
    void push_front(const T&);
  };


  template < class T>
  List<T>::List():
    head_(nullptr)
  {
    head_=ctFake();
    head_->next=nullptr;
  }
  template < class T>
  List<T>::~List()
  {
    clear();
  }
  template < class T >
  List<T>::List(const List< T >& list)
  {
    if (list.head_==nullptr) {
      head_=nullptr;
      return;
    }
    head_=ctFake();

    Node* curOld=list.head_->next;

    while (curOld!=nullptr)
    {
      head_->next=new Node*(curOld->val);
      head_=head_->next;
      curOld=curOld->next;
    }
  }

  template < class T >
  List< T >::List(List< T >&& other) noexcept :
    head_(other.head_)
  {
    other.head_ = nullptr;
    other.head->next=nullptr;
  }
  template < class T >
  LIter< T > List< T >::begin()
  {
    return LIter<T>(head_);
  }
  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter<T>(nullptr);
  }
  template < class T >
  void List< T >::clear()
  {
    LIter< T > it = begin();
    while (it != end()) {
      LIter< T > tmp(it.ptr->next);
      delete it.ptr;
      it = tmp;
    }
    head_ = nullptr;
  }
  template < class T >
  bool List< T >::empty() const
  {
    return begin() == end();
  }
}

#endif

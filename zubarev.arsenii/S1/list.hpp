#ifndef LIST_HPP
#define LIST_HPP
// #include "all.hpp"
#include "iter.hpp"
#include "c-iter.hpp"
namespace zubarev
{
  // template <class T>
  // class LIter;
  template < class T >
  struct Node
  {
    T val;
    Node* next;

    Node(const T& v, Node* n = nullptr) :
      val(v),
      next(n) {};
  };

  template < class T >
  class List
  {

  private:
    Node< T >* head_;
    Node< T >* ctFake()
    {
      Node< T >* el = new Node< T >{T(), nullptr};
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
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > begin();
    LIter< T > before_begin();
    LIter< T > end();

    LCIter< T > begin() const;
    LCIter< T > before_begin() const;
    LCIter< T > end() const;

    LCIter< T > cbegin() const;
    LCIter< T > cbefore_begin() const;
    LCIter< T > cend() const;

    void clear();
    bool empty() const;
    void pop_front();
    void push_front(const T&);
    void insert_after(LIter< T >, const T&);
    void erase_after(LIter< T >);
  };

  template < class T >
  List< T >::List()
  {
    head_ = ctFake();
    head_->next = nullptr;
  }

  template < class T >
  List< T >::~List()
  {
    clear();
    rmFake();
  }

  template < class T >
  List< T >::List(const List< T >& other)
  {
    if (other.head_ == nullptr) {
      head_ = nullptr;
      return;
    }
    head_ = ctFake();

    Node< T >* tmp = head_;

    Node< T >* curOld = other.head_->next;
    while (curOld != nullptr) {
      tmp->next = new Node< T >(curOld->val);
      tmp = tmp->next;
      curOld = curOld->next;
    }
    tmp->next=nullptr;

  }

  template < class T >
  List< T >::List(List< T >&& other) noexcept :
    head_(other.head_)
  {
  other.head_ = ctFake();  }
  template < class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this == &other) {
      return *this;
    }
    clear();
    Node< T >* tmp = head_;

    Node< T >* curOld = other.head_->next;
    while (curOld != nullptr) {
      tmp->next = new Node< T >(curOld->val);
      tmp = tmp->next;
      curOld = curOld->next;
    }
    tmp->next=nullptr;
    return *this;
  }

  template < class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    clear();
    rmFake();
    head_ = other.head_;
    other.head_ = ctFake();

    return *this;
  }
  template < class T >
  LIter< T > List< T >::before_begin()
  {
    return LIter< T >(head_);
  }
  template < class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(head_->next);
  }

  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }

  template < class T >
  LCIter< T > List< T >::before_begin() const
  {
    return LCIter< T >(head_);
  }
  template < class T >
  LCIter< T > List< T >::begin() const
  {
    return LCIter< T >(head_->next);
  }

  template < class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T >(nullptr);
  }

  template < class T >
  LCIter< T > List< T >::cbefore_begin() const
  {
    return before_begin();
  }
  template < class T >
  LCIter< T > List< T >::cbegin() const
  {
    return begin();
  }

  template < class T >
  LCIter< T > List< T >::cend() const
  {
    return end();
  }


  template < class T >
  void List< T >::clear()
  {
    Node< T >* cur = begin().ptr;
    while (cur != end().ptr) {
      Node< T >* curNext = cur->next;
      delete cur;
      cur = curNext;
    }

    head_->next = nullptr;
  }

  template < class T >
  bool List< T >::empty() const
  {
    return head_->next == nullptr;
  }

  template < class T >
  void List< T >::push_front(const T& val)
  {
    head_->next = new Node< T >(val, head_->next);
  }

  template < class T >
  void List< T >::pop_front()
  {
    if (empty()) {
      return;
    }
    Node< T >* toDel = head_->next;
    head_->next = toDel->next;
    delete toDel;
  }

  template < class T >
  void List< T >::insert_after(LIter< T > it, const T& val)
  {
    Node< T >* itNext = it.ptr->next;
    it.ptr->next = new Node< T >(val, itNext);
  }

  template < class T >
  void List< T >::erase_after(LIter< T > it)
  {
    Node< T >* itNext = it.ptr->next;
    it.ptr->next = itNext->next;
    delete itNext;
  }
}

#endif

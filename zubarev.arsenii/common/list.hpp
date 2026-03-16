#ifndef LIST_HPP
#define LIST_HPP
#include "c-iter.hpp"
#include "iter.hpp"
#include <utility>
namespace zubarev
{
  template < class T >
  struct Node
  {
    T val;
    Node* next;

    Node(const T& v, Node* n = nullptr):
      val(v),
      next(n) {};
  };

  template < class T >
  class List
  {

  private:
    Node< T >* head_;
    Node< T >* tail_;

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
    LIter< T > back();

    LCIter< T > begin() const;
    LCIter< T > before_begin() const;
    LCIter< T > end() const;
    LCIter< T > back() const;

    LCIter< T > cbegin() const;
    LCIter< T > cbefore_begin() const;
    LCIter< T > cend() const;
    LCIter< T > cback() const;

    void clear();
    bool empty() const;
    void pop_front();
    void push_front(const T&);
    void push_back(const T&);
    void insert_after(LIter< T >, const T&);
    void erase_after(LIter< T >);
  };

  template < class T >
  List< T >::List()
  {
    head_ = ctFake();
    tail_ = head_;
    head_->next = nullptr;
  }

  template < class T >
  List< T >::~List()
  {
    clear();
    if (head_) {
      rmFake();
    }
  }

  template < class T >
  List< T >::List(const List< T >& other)
  {
    if (other.head_ == nullptr) {
      head_ = nullptr;
      return;
    }
    head_ = ctFake();
    tail_ = head_;
    List< T > tempList;
    Node< T >* tmp = tempList.head_;

    Node< T >* curOld = other.head_->next;
    while (curOld != nullptr) {
      tmp->next = new Node< T >(curOld->val);
      tmp = tmp->next;
      curOld = curOld->next;
    }
    std::swap(head_, tempList.head_);
    tail_ = tmp;
  }

  template < class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(other.head_) tail_(other.tail_)
  {
    other.head_ = nullptr;
    other.tail_ = other.head_;
  }
  template < class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this == &other) {
      return *this;
    }
    clear();
    List< T > tempList;
    Node< T >* tmp = tempList.head_;

    Node< T >* curOld = other.head_->next;
    while (curOld != nullptr) {
      tmp->next = new Node< T >(curOld->val);
      tmp = tmp->next;
      curOld = curOld->next;
    }

    std::swap(head_, tempList.head_);
    tail_ = tmp;
    return *this;
  }

  template < class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    clear();
    if (head_) {
      rmFake();
    }
    head_ = other.head_;
    tail_ = other.tail_;
    other.head_ = nullptr;

    return *this;
  }
  template < class T >
  LIter< T > List< T >::before_begin()
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_);
  }
  template < class T >
  LIter< T > List< T >::begin()
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_->next);
  }

  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }
  template < class T >
  LIter< T > List< T >::back()
  {
    return Liter< T >(tail_);
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
  LCIter< T > List< T >::back() const
  {
    return LCiter< T >(tail_);
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
  LCIter< T > List< T >::cback() const
  {
    return back();
  }

  template < class T >
  void List< T >::clear()
  {
    if (!head_) {
      return;
    }
    Node< T >* cur = begin().ptr;
    while (cur != end().ptr) {
      Node< T >* curNext = cur->next;
      delete cur;
      cur = curNext;
    }

    head_->next = nullptr;
    tail_ = head_;
  }

  template < class T >
  bool List< T >::empty() const
  {
    if (!head_) {
      return true;
    }
    return head_->next == nullptr;
  }

  template < class T >
  void List< T >::push_front(const T& val)
  {
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }
    Node< T >* newNode = new Node< T >(val, head_->next);
    head_->next = newNode;
    if (tail_ == head_) {
      tail_ = newNode;
    }
  }

  template < class T >
  void List< T >::push_back(const T& val)
  {
    Node< T >* newNode = new Node< T >{val, nullptr};
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }
    tail_->next = newNode;
    tail_ = newNode;
  }

  template < class T >
  void List< T >::pop_front()
  {
    if (empty() || !head_) {
      return;
    }
    Node< T >* toDel = head_->next;
    head_->next = toDel->next;
    if (tail_ == toDel) {
      tail_ = head_;
    }
    delete toDel;
  }

  template < class T >
  void List< T >::insert_after(LIter< T > it, const T& val)
  {
    if (!it.ptr) {
      return;
    }
    Node< T >* itNext = it.ptr->next;
    it.ptr->next = new Node< T >(val, itNext);
    if (it.ptr == tail_) {
      tail_ = it.ptr->next;
    }
  }

  template < class T >
  void List< T >::erase_after(LIter< T > it)
  {
    if (!it.ptr || !it.ptr->next) {
      return;
    }
    Node< T >* itNext = it.ptr->next;
    if (tail_ == itNext) {
      tail_ = it.ptr;
    }
    it.ptr->next = itNext->next;
    delete itNext;
  }
}
#endif

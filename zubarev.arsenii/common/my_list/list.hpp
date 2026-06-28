#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include "c-iter.hpp"
#include "iter.hpp"

namespace zubarev
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T val;
      Node* next;

      Node(const T& v, Node* n = nullptr);
      Node(T&& v, Node* n = nullptr);
    };
  }

  template< class T >
  class List
  {
  public:
    List();
    ~List() noexcept;
    List(const List& list);
    List(List&& list) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > begin() noexcept;
    LIter< T > before_begin() noexcept;
    LIter< T > end() noexcept;
    LIter< T > back();

    LCIter< T > begin() const noexcept;
    LCIter< T > before_begin() const noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > back() const;

    LCIter< T > cbegin() const noexcept;
    LCIter< T > cbefore_begin() const noexcept;
    LCIter< T > cend() const noexcept;
    LCIter< T > cback() const;

    void clear() noexcept;
    bool empty() const noexcept;
    void pop_front() noexcept;

    void push_front(const T& val);
    void push_front(T&& val);
    void push_back(const T& val);

    LIter< T > insert_after(LIter< T > it, const T& val);
    LIter< T > insert_after(LIter< T > it, T&& val);

    void erase_after(LIter< T > it) noexcept;

  private:
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    detail::Node< T >* ctFake();
    void rmFake();
  };

  template< class T >
  detail::Node< T >* List< T >::ctFake()
  {
    return new detail::Node< T >{T(), nullptr};
  }

  template< class T >
  detail::Node< T >::Node(const T& v, Node* n):
    val(v),
    next(n)
  {}

  template< class T >
  detail::Node< T >::Node(T&& v, Node* n):
    val(std::move(v)),
    next(n)
  {}

  template< class T >
  void List< T >::rmFake()
  {
    delete head_;
  }

  template< class T >
  List< T >::List():
    head_(ctFake()),
    tail_(head_)
  {
    head_->next = nullptr;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    if (head_) {
      rmFake();
    }
  }

  template< class T >
  List< T >::List(const List< T >& other):
    head_(nullptr),
    tail_(head_)
  {
    List< T > tempList;
    auto it = tempList.before_begin();
    detail::Node< T >* curOld = other.head_->next;

    while (curOld != nullptr) {
      it = tempList.insert_after(it, curOld->val);
      curOld = curOld->next;
    }

    std::swap(head_, tempList.head_);
    std::swap(tail_, tempList.tail_);
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(std::exchange(other.head_, nullptr)),
    tail_(std::exchange(other.tail_, nullptr))
  {}

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != std::addressof(other)) {
      List< T > temp(other);
      std::swap(head_, temp.head_);
      std::swap(tail_, temp.tail_);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    clear();
    if (head_) {
      rmFake();
    }
    head_ = std::exchange(other.head_, nullptr);
    tail_ = std::exchange(other.tail_, nullptr);

    return *this;
  }

  template< class T >
  LIter< T > List< T >::before_begin() noexcept
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_->next);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LIter< T > List< T >::back()
  {
    if (empty()) {
      return end();
    }
    return LIter< T >(tail_);
  }

  template< class T >
  LCIter< T > List< T >::before_begin() const noexcept
  {
    if (!head_) {
      return end();
    }
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    if (!head_) {
      return end();
    }
    return LCIter< T >(head_->next);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::back() const
  {
    if (empty()) {
      return end();
    }
    return LCIter< T >(tail_);
  }

  template< class T >
  LCIter< T > List< T >::cbefore_begin() const noexcept
  {
    return before_begin();
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return begin();
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return end();
  }

  template< class T >
  LCIter< T > List< T >::cback() const
  {
    return back();
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    if (!head_) {
      return;
    }

    while (head_->next) {
      erase_after(before_begin());
    }

    tail_ = head_;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    if (!head_) {
      return true;
    }
    return head_->next == nullptr;
  }

  template< class T >
  void List< T >::push_front(const T& val)
  {
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }
    detail::Node< T >* newNode = new detail::Node< T >(val, head_->next);
    head_->next = newNode;
    if (tail_ == head_) {
      tail_ = newNode;
    }
  }

  template< class T >
  void List< T >::push_front(T&& val)
  {
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }
    detail::Node< T >* newNode = new detail::Node< T >(std::move(val), head_->next);
    head_->next = newNode;
    if (tail_ == head_) {
      tail_ = newNode;
    }
  }

  template< class T >
  void List< T >::push_back(const T& val)
  {
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }

    detail::Node< T >* newNode = new detail::Node< T >(val, nullptr);
    tail_->next = newNode;
    tail_ = newNode;
  }

  template< class T >
  void List< T >::pop_front() noexcept
  {
    if (empty() || !head_) {
      return;
    }
    detail::Node< T >* toDel = head_->next;
    head_->next = toDel->next;
    if (tail_ == toDel) {
      tail_ = head_;
    }
    delete toDel;
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > it, const T& val)
  {
    if (!it.ptr_) {
      return end();
    }
    detail::Node< T >* itNext = it.ptr_->next;
    detail::Node< T >* newNode = new detail::Node< T >(val, itNext);
    it.ptr_->next = newNode;

    if (tail_ == it.ptr_) {
      tail_ = newNode;
    }
    return LIter< T >(newNode);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > it, T&& val)
  {
    if (!it.ptr_) {
      return end();
    }
    detail::Node< T >* itNext = it.ptr_->next;
    detail::Node< T >* newNode = new detail::Node< T >(std::move(val), itNext);
    it.ptr_->next = newNode;

    if (tail_ == it.ptr_) {
      tail_ = newNode;
    }
    return LIter< T >(newNode);
  }

  template< class T >
  void List< T >::erase_after(LIter< T > it) noexcept
  {
    if (!it.ptr_ || !it.ptr_->next) {
      return;
    }
    detail::Node< T >* itNext = it.ptr_->next;
    it.ptr_->next = itNext->next;

    if (tail_ == itNext) {
      tail_ = it.ptr_;
    }
    delete itNext;
  }
}
#endif

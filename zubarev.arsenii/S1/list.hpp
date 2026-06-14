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

    LCIter< T > begin() const noexcept;
    LCIter< T > before_begin() const noexcept;
    LCIter< T > end() const noexcept;

    LCIter< T > cbegin() const noexcept;
    LCIter< T > cbefore_begin() const noexcept;
    LCIter< T > cend() const noexcept;

    void clear() noexcept;
    bool empty() const noexcept;
    void pop_front() noexcept;

    void push_front(const T& val);
    void push_front(T&& val);

    LIter< T > insert_after(LIter< T > it, const T& val);
    LIter< T > insert_after(LIter< T > it, T&& val);

    void erase_after(LIter< T >) noexcept;

  private:
    detail::Node< T >* head_;
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
    val(std::forward< T >(v)),
    next(n)
  {}
  template< class T >
  void List< T >::rmFake()
  {
    delete head_;
  }
  template< class T >
  List< T >::List():
    head_(ctFake())
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
    head_(nullptr)
  {
    try {
      head_ = ctFake();
      List< T > tempList;
      auto it = tempList.before_begin();
      detail::Node< T >* curOld = other.head_->next;

      while (curOld != nullptr) {
        it = tempList.insert_after(it, curOld->val);
        curOld = curOld->next;
      }

      std::swap(head_, tempList.head_);
    } catch (...) {
      delete head_;
      throw;
    }
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(std::exchange(other.head_, nullptr))
  {}
  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {

    if (this != std::addressof(other)) {
      List< T > temp(other);
      std::swap(head_, temp.head_);
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

    rmFake();

    head_ = std::exchange(other.head_, nullptr);

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
  LCIter< T > List< T >::before_begin() const noexcept
  {
    return LCIter< T >(head_);
  }
  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(head_->next);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr);
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
  void List< T >::clear() noexcept
  {
    if (!head_) {
      return;
    }

    while (head_->next) {
      erase_after(before_begin());
    }
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
    }
    head_->next = new detail::Node< T >(val, head_->next);
  }

  template< class T >
  void List< T >::push_front(T&& val)
  {
    if (!head_) {
      head_ = ctFake();
    }
    head_->next = new detail::Node< T >(std::move(val), head_->next);
  }

  template< class T >
  void List< T >::pop_front() noexcept
  {
    if (empty() || !head_) {
      return;
    }
    detail::Node< T >* toDel = head_->next;
    head_->next = toDel->next;
    delete toDel;
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > it, const T& val)
  {
    if (!it.ptr_) {
      return end();
    }
    detail::Node< T >* itNext = it.ptr_->next;
    it.ptr_->next = new detail::Node< T >(val, itNext);
    return LIter< T >(it.ptr_->next);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > it, T&& val)
  {
    if (!it.ptr_) {
      return end();
    }
    detail::Node< T >* itNext = it.ptr_->next;
    it.ptr_->next = new detail::Node< T >(std::move(val), itNext);
    return LIter< T >(it.ptr_->next);
  }

  template< class T >
  void List< T >::erase_after(LIter< T > it) noexcept
  {
    if (!it.ptr_ || !it.ptr_->next) {
      return;
    }
    detail::Node< T >* itNext = it.ptr_->next;
    it.ptr_->next = itNext->next;
    delete itNext;
  }
}

#endif

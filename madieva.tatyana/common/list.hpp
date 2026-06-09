#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <cassert>
#include <utility>
#include <stdexcept>
#include <memory>

namespace madieva {
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

  namespace detail {
    template< class T >
    struct node_t {
      T val_;
      node_t< T > * next_;
      node_t< T > * prev_;
    };
  }
  template< class T >
  class LIter {
  public:
    LIter< T > & operator++() noexcept;
    LIter< T > & operator--() noexcept;
    LIter< T > operator++(int) noexcept;
    LIter< T > operator--(int) noexcept;
    bool operator==(const LIter< T > & a) const noexcept;
    bool operator!=(const LIter< T > & a) const noexcept;
    T & operator*() noexcept;
    T * operator->() noexcept;
  private:
    friend class List< T >;
    detail::node_t< T > * it_;
    detail::node_t< T > * head_;
    LIter(detail::node_t< T > * a, detail::node_t< T > * h) noexcept;
  };

  template< class T >
  class LCIter {
  public:
    LCIter< T > & operator++() noexcept;
    LCIter< T > & operator--() noexcept;
    LCIter< T > operator++(int) noexcept;
    LCIter< T > operator--(int) noexcept;
    bool operator==(const LCIter< T > & a) const noexcept;
    bool operator!=(const LCIter< T > & a) const noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
  private:
    friend class List< T >;
    const detail::node_t< T > * it_;
    const detail::node_t< T > * head_;
    LCIter(const detail::node_t< T > * a, detail::node_t< T > * h) noexcept;
  };

  template< class T >
  class List {
  public:
    List() noexcept;
    List(const List & a);
    List(List && other) noexcept;
    ~List() noexcept;
    List & operator=(const List & a);
    List & operator=(List && other) noexcept;
    void clear() noexcept;
    void pushFront(const T & a);
    void pushFront(T && a);
    void pushBack(const T & a);
    void pushBack(T && a);
    void popFront() noexcept;
    void popBack() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;
    void swap(List& other) noexcept;
    void splice(LIter< T > pos, List< T > & other) noexcept;
    void splice(LIter< T > pos, List< T > & other, LIter< T > it) noexcept;
    void splice(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last) noexcept;
    void splice(LIter< T > pos, List< T > & other, LIter< T > it, size_t size);
    void sort();
    template< class Compare >
    void sort(Compare cmp);
    void merge(List< T > & other) noexcept;
    template< class Compare >
    void merge(List< T > & other, Compare cmp) noexcept;
    LIter< T > partition(const T & pivot);
    template< class Predicate >
    LIter< T > partition(Predicate pred);
  private:
    detail::node_t< T > * head_;
    size_t size_;
  };

  template< class T >
  LIter< T >::LIter(detail::node_t< T > * a, detail::node_t< T > * h) noexcept:
    it_(a),
    head_(h)
  {}

  template< class T >
  LIter< T > & LIter< T >::operator++() noexcept
  {
    assert(it_ != nullptr);
    if (it_->next_ == head_) {
      it_ = nullptr;
    } else {
      it_ = it_->next_;
    }
    return *this;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--() noexcept
  {
    assert(it_ != head_);
    if (it_ == nullptr) {
      it_ = head_->prev_;
    } else {
      it_ = it_->prev_;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    assert(it_ != nullptr);
    LIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    assert(it_ != head_);
    LIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & a) const noexcept
  {
    return (it_ == a.it_);
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & a) const noexcept
  {
    return (it_ != a.it_);
  }

  template< class T >
  T & LIter< T >::operator*() noexcept
  {
    assert(it_);
    return it_->val_;
  }

  template< class T >
  T * LIter< T >::operator->() noexcept
  {
    assert(it_);
    return std::addressof(it_->val_);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::node_t< T > * a,  detail::node_t< T > * h) noexcept:
    it_(a),
    head_(h)
  {}

  template< class T >
  LCIter< T > & LCIter< T >::operator++() noexcept
  {
    assert(it_ != nullptr);
    if (it_->next_ == head_) {
      it_ = nullptr;
    } else {
      it_ = it_->next_;
    }
    return *this;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--() noexcept
  {
    assert(it_ != head_);
    if (it_ == nullptr) {
      it_ = head_->prev_;
    } else {
      it_ = it_->prev_;
    }
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    assert(it_ != head_);
    LCIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    assert(it_ != head_);
    LCIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & a) const noexcept
  {
    return (it_ == a.it_);
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & a) const noexcept
  {
    return (it_ != a.it_);
  }

  template< class T >
  const T & LCIter< T >::operator*() const noexcept
  {
    assert(it_);
    return it_->val_;
  }

  template< class T >
  const T * LCIter< T >::operator->() const noexcept
  {
    assert(it_);
    return std::addressof(it_->val_);
  }

  template< class T >
  List< T >::List() noexcept:
    head_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T >::List(const List< T > & a):
    head_(nullptr),
    size_(0)
  {
    for (LCIter< T > it = a.begin(); it != a.end(); ++it) {
      try {
        pushBack(*it);
      } catch (...) {
        clear();
        throw;
      }
    }
  }

  template< class T >
  List< T >::List(List && other) noexcept:
    head_(other.head_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & a)
  {
    assert(this != &a);
    List< T > tmp(a);
    swap(tmp);
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List && other) noexcept
  {
    if (this != &other) {
      clear();
      head_ = other.head_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    if (head_) {
      detail::node_t< T > * end = head_->prev_;
      while (head_ != end) {
        detail::node_t< T > * temp = head_->next_;
        delete head_;
        head_ = temp;
      }
      delete head_;
      head_ = nullptr;
      size_ = 0;
    }
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head_, head_);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(head_, head_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(head_, head_);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr, head_);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr, head_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr, head_);
  }

  template< class T >
  void List< T >::pushFront(const T & a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{a, nullptr, nullptr};
      head_->next_ = head_;
      head_->prev_ = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{a, head_, head_->prev_};
      head_->prev_->next_ = temp;
      head_->prev_ = temp;
      head_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushFront(T && a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{std::forward<T>(a), nullptr, nullptr};
      head_->next_ = head_;
      head_->prev_ = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{std::forward<T>(a), head_, head_->prev_};
      head_->prev_->next_ = temp;
      head_->prev_ = temp;
      head_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushBack(const T & a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{a, nullptr, nullptr};
      head_->next_ = head_;
      head_->prev_ = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{a, head_, head_->prev_};
      head_->prev_->next_ = temp;
      head_->prev_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushBack(T && a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{std::forward<T>(a), nullptr, nullptr};
      head_->next_ = head_;
      head_->prev_ = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{std::forward<T>(a), head_, head_->prev_};
      head_->prev_->next_ = temp;
      head_->prev_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (size_) {
      if (size_ == 1) {
        delete head_;
        head_ = nullptr;
        size_ = 0;
      } else {
        head_->next_->prev_ = head_->prev_;
        head_->prev_->next_ = head_->next_;
        detail::node_t< T > * a = head_->next_;
        delete head_;
        head_ = a;
        size_--;
      }
    }
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (size_) {
      if (size_ == 1) {
        delete head_;
        head_ = nullptr;
        size_ = 0;
      } else {
        detail::node_t< T > * a = head_->prev_;
        a->next_->prev_ = a->prev_;
        a->prev_->next_ = a->next_;
        delete a;
        size_--;
      }
    }
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other) noexcept
  {
    splice(pos, other, other.begin(), other.end());
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other, LIter< T > it) noexcept
  {
    if (other.empty() || it == other.end()) {
      return;
    }
    detail::node_t< T > * node = it.it_;
    detail::node_t< T > * prev_node = node->prev_;
    detail::node_t< T > * next_node = node->next_;

    prev_node->next_ = next_node;
    next_node->prev_ = prev_node;
    if (other.head_ == node) {
      if (other.size_ == 1) {
        other.head_ = nullptr;
      } else {
        other.head_ = next_node;
      }
    }
    if (pos.it_ == nullptr) {
      if (head_ == nullptr) {
        head_ = node;
        node->next_ = node;
        node->prev_ = node;
      } else {
        detail::node_t< T > * tail = head_->prev_;
        node->next_ = head_;
        node->prev_ = tail;
        tail->next_ = node;
        head_->prev_ = node;
      }
    } else {
      node->next_ = pos.it_;
      node->prev_ = pos.it_->prev_;
      pos.it_->prev_->next_ = node;
      pos.it_->prev_ = node;
      if (head_ == pos.it_) {
        head_ = node;
      }
    }
    size_++;
    other.size_--;
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last) noexcept
  {
    while (first != last) {
      LIter< T > next = first;
      ++next;
      splice(pos, other, first);
      first = next;
    }
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other, LIter< T > it, size_t size)
  {
    if (size == 0) {
      return;
    }
    if (it == other.end() || other.size_ < size) {
      throw std::out_of_range("not enough elements in other");
    }
    for (size_t i = 0; i < size; ++i) {
      LIter< T > next = it;
      ++next;
      splice(pos, other, it);
      it = next;
      ++pos;
    }
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare cmp)
  {
    if (size_ < 2) {
      return;
    }

    for (LIter< T > i = begin(); i != end(); ++i) {
      LIter< T > min = i;

      for (LIter< T > j = i; j != end(); ++j) {
        if (cmp(*j, *min)) {
          min = j;
        }
      }

      std::swap(*i, *min);
    }
  }

  template< class T >
  void List< T >::merge(List< T > & other) noexcept
  {
    merge(other, std::less< T >());
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List< T > & other, Compare cmp) noexcept
  {
    if (this == & other) {
      return;
    }

    LIter< T > it = begin();
    LIter< T > it_other = other.begin();

    while (it_other != other.end()) {
      if (it == end() || !cmp(*it, *it_other)) {
        LIter< T > next = it_other;
        ++next;
        splice(it, other, it_other);
        it_other = next;
      } else {
        ++it;
      }
    }
  }

  template< class T >
  LIter< T > List< T >::partition(const T & pivot)
  {
    return partition([&pivot](const T & val)
    {
      return std::less< T >()(val, pivot);
    });
  }

  template< class T >
  template< class Predicate >
  LIter< T > List< T >::partition(Predicate pred)
  {
    LIter< T > it = begin();
    for (LIter< T > scan = begin(); scan != end(); ++scan) {
      if (pred(*scan)) {
        std::swap(*it, *scan);
        ++it;
      }
    }
    return it;
  }
}

#endif

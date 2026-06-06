#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <cassert>
#include <utility>

namespace madieva {
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

  namespace detail {
    template< class T >
    struct node_t {
      T val;
      node_t< T > * next;
      node_t< T > * prev;
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
    detail::node_t< T > * it;
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
    const detail::node_t< T > * it;
    const detail::node_t< T > * head_;
    LCIter(const detail::node_t< T > * a, detail::node_t< T > * h) noexcept;
  };

  template< class T >
  class List {
  public:
    List() noexcept;
    List(const List& a);
    ~List() noexcept;
    List & operator=(const List & a);
    void clear() noexcept;
    void pushFront(const T & a);
    void pushFront(T && a);
    void pushBack(const T & a);
    void pushBack(T && a);
    void popFront() noexcept;
    void popBack() noexcept;
    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;
    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;
    void swap(List& other) noexcept;
    void splice(LIter< T > pos, List< T > & other);
    void splice(LIter< T > pos, List< T > & other, LIter< T > it);
    void splice(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last);
    void splice(LIter< T > pos, List< T > & other, LIter< T > it, size_t size);
    void sort();
    void sort(Compare cmp);
    void merge(List< T > & other);
    template<class Compare>
    void merge(List< T > & other, Compare cmp);
    LIter< T > partition(const T & pivot);
    template< class Predicate >
    LIter< T > partition(Predicate pred);
  private:
    detail::node_t< T > * head_;
    size_t size_;
  };

  template< class T >
  LIter< T >::LIter(detail::node_t< T > * a, detail::node_t< T > * h) noexcept :
    it(a),
    head_(h)
  {}

  template< class T >
  LIter< T > & LIter< T >::operator++() noexcept
  {
    assert(it != nullptr);
    if (it->next == head_) {
      it = nullptr;
    } else {
      it = it->next;
    }
    return *this;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--() noexcept
  {
    assert(it != head_);
    if (it == nullptr) {
      it = head_->prev;
    } else {
      it = it->prev;
    }
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    assert(it != nullptr);
    LIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    assert(it != head_);
    LIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & a) const noexcept
  {
    return(it == a.it);
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & a) const noexcept
  {
    return(it != a.it);
  }

  template< class T >
  T & LIter< T >::operator*() noexcept
  {
    assert(it);
    return it->val;
  }

  template< class T >
  T * LIter< T >::operator->() noexcept
  {
    assert(it);
    return &(it->val);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::node_t< T > * a,  detail::node_t< T > * h) noexcept :
    it(a),
    head_(h)
  {}

  template< class T >
  LCIter< T > & LCIter< T >::operator++() noexcept
  {
    assert(it != nullptr);
    if (it->next == head_) {
      it = nullptr;
    } else {
      it = it->next;
    }
    return *this;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--() noexcept
  {
    assert(it != head_);
    if (it == nullptr) {
      it = head_->prev;
    } else {
      it = it->prev;
    }
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    assert(it != head_);
    LCIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    assert(it != head_);
    LCIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & a) const noexcept
  {
    return(it == a.it);
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & a) const noexcept
  {
    return(it != a.it);
  }

  template< class T >
  const T & LCIter< T >::operator*() const noexcept
  {
    assert(it);
    return it->val;
  }

  template< class T >
  const T * LCIter< T >::operator->() const noexcept
  {
    assert(it);
    return &(it->val);
  }

  template< class T >
  List< T >::List() noexcept :
    head_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T >::List(const List< T > & a) :
    head_(nullptr),
    size_(0)
  {
    for (LCIter< T > it = a.begin(); it != a.end(); ++it) {
      try {
        this->pushBack(*it);
      } catch (...) {
        this->clear();
        throw;
      }
    }
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & a)
  {
    assert(this != & a);
    List< T > tmp(a);
    this->swap(tmp);
    return *this;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    if (head_) {
      detail::node_t< T > * end = head_->prev;
      while (head_ != end) {
        detail::node_t< T > * temp = head_->next;
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
      head_->next = head_;
      head_->prev = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{a, head_, head_->prev};
      head_->prev->next = temp;
      head_->prev = temp;
      head_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushFront(T && a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{std::move(a), nullptr, nullptr};
      head_->next = head_;
      head_->prev = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{std::move(a), head_, head_->prev};
      head_->prev->next = temp;
      head_->prev = temp;
      head_ = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushBack(const T & a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{a, nullptr, nullptr};
      head_->next = head_;
      head_->prev = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{a, head_, head_->prev};
      head_->prev->next = temp;
      head_->prev = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pushBack(T && a)
  {
    if (!head_) {
      head_ = new detail::node_t< T >{std::move(a), nullptr, nullptr};
      head_->next = head_;
      head_->prev = head_;
      size_ = 1;
    } else {
      detail::node_t< T > * temp = new detail::node_t< T >{std::move(a), head_, head_->prev};
      head_->prev->next = temp;
      head_->prev = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if(size_) {
      if(size_ == 1) {
        delete head_;
        head_ = nullptr;
        size_ = 0;
      } else {
        head_->next->prev = head_->prev;
        head_->prev->next = head_->next;
        detail::node_t< T > * a = head_->next;
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
      if(size_ == 1) {
        delete head_;
        head_ = nullptr;
        size_ = 0;
      } else{
        detail::node_t< T > * a = head_->prev;
        a->next->prev = a->prev;
        a->prev->next = a->next;
        delete a;
        size_--;
      }
    }
  }

  template< class T >
  size_t List< T >::getSize() const noexcept
  {
    return size_;
  }

  template< class T >
  bool List< T >::isEmpty() const noexcept
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
  void List< T >::splice(LIter< T > pos, List< T > & other)
  {
    splice(pos, other, other.begin(), other.end());
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other, LIter< T > it)
  {
    if (other.isEmpty() || it == other.end()) {
      return;
    }
    detail::node_t< T > * node = it.it;
    detail::node_t< T > * prev_node = node->prev;
    detail::node_t< T > * next_node = node->next;

    prev_node->next = next_node;
    next_node->prev = prev_node;
    if (other.head_ == node) {
      if (other.size_ == 1) {
        other.head_ = nullptr;
      } else {
        other.head_ = next_node;
      }
    }
    if (pos.it == nullptr) {
      if (head_ == nullptr) {
        head_ = node;
        node->next = node;
        node->prev = node;
      } else {
        detail::node_t< T > * tail = head_->prev;
        node->next = head_;
        node->prev = tail;
        tail->next = node;
        head_->prev = node;
      }
    } else {
      node->next = pos.it;
      node->prev = pos.it->prev;
      pos.it->prev->next = node;
      pos.it->prev = node;
      if (head_ == pos.it) {
        head_ = node;
      }
    }
    size_++;
    other.size_--;
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last)
  {
    while (first != last) {
      LIter< T > next = first;
      ++next;
      splice(pos, other, first);
      first = next;
      ++pos;
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
}

#endif

#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include <cstddef>
#include <limits>
#include <cassert>

namespace sedov
{
  const size_t MAX = std::numeric_limits< size_t >::max();

  template< class T >
  class List;

  namespace detail
  {
    template< class T >
    struct Node
    {
      T val_;
      Node< T > * next_;
      Node< T > * prev_;
      Node(const T & value);
      Node(T && value);
    };
  }

  template< class T >
  class LIter
  {
  public:
    LIter(detail::Node< T > * p = nullptr);

    T & operator*();
    T * operator->();

    LIter & operator++();
    LIter operator++(int);
    LIter & operator--();
    LIter operator--(int);

    bool operator==(const LIter & other) const;
    bool operator!=(const LIter & other) const;
  private:
    friend class List< T >;
    detail::Node< T > * ptr_;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter(const detail::Node< T > * p = nullptr);

    const T & operator*() const;
    const T * operator->() const;

    LCIter & operator++();
    LCIter operator++(int);
    LCIter & operator--();
    LCIter operator--(int);

    bool operator==(const LCIter & other) const;
    bool operator!=(const LCIter & other) const;
  private:
    friend class List< T >;
    const detail::Node< T > * ptr_;
  };

  template< class T >
  class List
  {
  public:
    List();
    List(const List & h);
    List(List && h) noexcept;
    ~List() noexcept;

    List & operator=(const List & h);
    List & operator=(List && h) noexcept;

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    void pushFront(const T & v);
    void pushBack(const T & v);
    LIter< T > insert(LIter< T > p, const T & v);

    void popFront();
    void popBack();
    LIter< T > erase(LIter< T > p);

    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List & h) noexcept;

    void splice(LIter<T> pos, List& other) noexcept;
    void splice(LIter<T> pos, List& other, LIter<T> it) noexcept;
    void splice(LIter<T> pos, List& other, LIter<T> first, LIter<T> last) noexcept;
    void merge(List& other) noexcept;
    void sort();

    template<class P>
    LIter<T> partition(P p);
  private:
    detail::Node< T > * head_;
    detail::Node< T > * tail_;
    size_t size_;
  };

  template< class T >
  detail::Node< T >::Node(const T & value):
    val_(value),
    next_(nullptr),
    prev_(nullptr)
  {}

  template< class T >
  detail::Node< T >::Node(T && value):
    val_(value),
    next_(nullptr),
    prev_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T > * p):
    ptr_(p)
  {}

  template< class T >
  T & LIter< T >::operator*()
  {
    return ptr_->val_;
  }

  template< class T >
  T * LIter< T >::operator->()
  {
    return &ptr_->val_;
  }

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter temp = *this;
    ptr_ = ptr_->next_;
    return temp;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--()
  {
    ptr_ = ptr_->prev_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter temp = *this;
    ptr_ = ptr_->prev_;
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter & other) const
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter & other) const
  {
    return !(ptr_ == other.ptr_);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T > * p):
    ptr_(p)
  {}

  template< class T >
  const T & LCIter< T >::operator*() const
  {
    return ptr_->val_;
  }

  template< class T >
  const T * LCIter< T >::operator->() const
  {
    return &ptr_->val_;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--()
  {
    ptr_ = ptr_->prev_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter tmp = *this;
    ptr_ = ptr_->prev_;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter & other) const
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter & other) const
  {
    return !(ptr_ == other.ptr_);
  }

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List & h):
    List()
  {
    for (detail::Node< T > * cur = h.head_; cur != nullptr; cur = cur->next_)
    {
      pushBack(cur->val_);
    }
  }

  template< class T >
  List< T >::List(List && h) noexcept:
    head_(std::exchange(h.head_, nullptr)),
    tail_(std::exchange(h.tail_, nullptr)),
    size_(std::exchange(h.size_, 0))
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T > & List< T >::operator=(const List & h)
  {
    assert(this != &h);

    List temp(h);
    swap(temp);
    return *this;
  }

  template<class T>
  List< T > & List< T >::operator=(List && h) noexcept
  {
    assert(this != &h);
    clear();
    head_ = std::exchange(h.head_, nullptr);
    tail_ = std::exchange(h.tail_, nullptr);
    size_ = std::exchange(h.size_, 0);
    return *this;
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  T & List< T >::front()
  {
    return head_->val_;
  }

  template< class T >
  const T & List< T >::front() const
  {
    return head_->val_;
  }

  template< class T >
  T & List< T >::back()
  {
    return tail_->val_;
  }

  template< class T >
  const T & List< T >::back() const
  {
    return tail_->val_;
  }

  template< class T >
  void List< T >::pushFront(const T & v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(v);
    newNode->next_ = head_;
    if (head_)
    {
      head_->prev_ = newNode;
    }
    else
    {
      tail_ = newNode;
    }
    head_ = newNode;
    ++size_;
  }

  template< class T >
  void List< T >::pushBack(const T & v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(v);
    newNode->prev_ = tail_;
    if (tail_)
    {
      tail_->next_ = newNode;
    }
    else
    {
      head_ = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > p, const T & v)
  {
    if (!p.ptr_)
    {
      pushBack(v);
      return LIter< T >(tail_);
    }
    if (p.ptr_ == head_)
    {
      pushFront(v);
      return LIter< T >(head_);
    }
    detail::Node< T > * newNode = new detail::Node< T >(v);
    detail::Node< T > * next = p.ptr_;
    detail::Node< T > * prev = next->prev_;
    newNode->prev_ = prev;
    newNode->next_ = next;
    prev->next_ = newNode;
    next->prev_ = newNode;
    ++size_;
    return LIter< T >(newNode);
  }

  template< class T >
  void List< T >::popFront()
  {
    if (!head_)
    {
      return;
    }
    detail::Node< T > * temp = head_;
    head_ = head_->next_;
    if (head_)
    {
      head_->prev_ = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }
    delete temp;
    --size_;
  }

  template< class T >
  void List< T >::popBack()
  {
    if (!tail_)
    {
      return;
    }
    detail::Node< T > * temp = tail_;
    tail_ = tail_->prev_;
    if (tail_)
    {
      tail_->next_ = nullptr;
    }
    else
    {
      head_ = nullptr;
    }
    delete temp;
    --size_;
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > p)
  {
    if (!p.ptr_)
    {
      return end();
    }
    detail::Node< T > * next = p.ptr_->next_;
    detail::Node< T > * prev = p.ptr_->prev_;
    if (prev)
    {
      prev->next_ = next;
    }
    else
    {
      head_ = next;
    }
    if (next)
    {
      next->prev_ = prev;
    }
    else
    {
      tail_ = prev;
    }
    delete p.ptr_;
    --size_;
    return LIter< T >(next);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (head_)
    {
      detail::Node< T > * temp = head_;
      head_ = head_->next_;
      delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::swap(List & h) noexcept
  {
    std::swap(head_, h.head_);
    std::swap(tail_, h.tail_);
    std::swap(size_, h.size_);
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List & other) noexcept
  {
    if (other.size_ == 0 || this == &other)
    {
      return;
    }
    splice(pos, other, other.begin(), other.end());
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List & other, LIter< T > it) noexcept
  {
    if (it.ptr_ == nullptr || this == &other && pos.ptr_ == it.ptr_)
    {
      return;
    }
    LIter< T > next = it;
    ++next;
    splice(pos, other, it, next);
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List & other, LIter< T > first, LIter< T > last) noexcept
  {
    if (first == last || other.size_ == 0)
    {
      return;
    }
    size_t count = 0;
    for (LIter< T > it = first; it != last; ++it)
    {
      ++count;
    }
    detail::Node< T > * first_node = first.ptr_;
    detail::Node< T > * last_node = (last.ptr_ == nullptr) ? other.tail_ : last.ptr_->prev_;
    if (first_node->prev_)
    {
      first_node->prev_->next_ = last.ptr_;
    }
    else
    {
      other.head_ = last.ptr_;
    }
    if (last.ptr_)
    {
      last.ptr_->prev_ = first_node->prev_;
    }
    else
    {
      other.tail_ = first_node->prev_;
    }
    detail::Node< T > * pos_node = pos.ptr_;
    detail::Node< T > * pos_next = (pos_node) ? pos_node->next_ : head_;
    first_node->prev_ = pos_node;
    last_node->next_ = pos_next;
    if (pos_node)
    {
      pos_node->next_ = first_node;
    }
    else
    {
      head_ = first_node;
    }
    if (pos_next)
    {
      pos_next->prev_ = last_node;
    }
    else
    {
      tail_ = last_node;
    }
    size_ += count;
    other.size_ -= count;
  }

  template< class T >
  void List< T >::merge(List & other) noexcept
  {
    if (this == &other || other.size_ == 0)
    {
      return;
    }
    LIter< T > this_it = begin();
    LIter< T > other_it = other.begin();
    while (this_it != end() && other_it != other.end())
    {
      if (other_it.ptr_->val_ < this_it.ptr_->val_)
      {
        LIter< T > to_move = other_it;
        ++other_it;
        splice(this_it, other, to_move);
      }
      else
      {
        ++this_it;
      }
    }
    if (other_it != other.end())
    {
      splice(end(), other, other_it, other.end());
    }
  }

  template< class T >
  static void mergeSort(List< T > & list)
  {
    if (list.size() <= 1)
    {
      return;
    }
    List< T > right;
    size_t half = list.size() / 2;
    LIter< T > mid = list.begin();
    for (size_t i = 0; i < half; ++i)
    {
      ++mid;
    }
    right.splice(right.begin(), list, mid, list.end());
    mergeSort(list);
    mergeSort(right);
    list.merge(right);
  }

  template< class T >
  void List< T >::sort()
  {
    if (size_ <= 1)
    {
      return;
    }
    mergeSort(*this);
  }

  template< class T >
  template< class P >
  LIter< T > List< T >::partition(P p)
  {
    if (size_ == 0)
    {
      return end();
    }
    List< T > false_list;
    LIter< T > it = begin();
    while (it != end())
    {
      if (!p(*it))
      {
        LIter< T > to_move = it;
        ++it;
        false_list.splice(false_list.end(), *this, to_move);
      }
      else
      {
        ++it;
      }
    }
    splice(end(), false_list);
    LIter< T > result = begin();
    for (size_t i = 0; i < size_ - false_list.size(); ++i)
    {
      ++result;
    }
    return result;
  }
}

#endif

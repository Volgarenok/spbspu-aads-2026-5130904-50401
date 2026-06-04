#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include <cstddef>
#include <limits>
#include <cassert>
#include <new>

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
      alignas(T) unsigned char storage_[sizeof(T)];
      Node< T > * next_;
      Node< T > * prev_;
      Node(const T & value);
      Node(T && value);
      template< class... Args >
      explicit Node(Args&&... args);
      ~Node();
      T & getValue();
      const T & getValue() const;
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

    bool operator==(const LIter & h) const;
    bool operator!=(const LIter & h) const;
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

    bool operator==(const LCIter & h) const;
    bool operator!=(const LCIter & h) const;
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
    void pushFront(T && v);
    void pushBack(const T & v);
    void pushBack(T && v);

    template< class... Args >
    LIter< T > emplaceFront(Args&&... args);

    template< class... Args >
    LIter< T > emplaceBack(Args&&... args);

    template< class... Args >
    LIter< T > emplace(LIter< T > p, Args&&... args);

    template< class... Args >
    LIter< T > emplaceAfter(LIter< T > p, Args&&... args);

    LIter< T > insert(LIter< T > p, const T & v);

    void popFront();
    void popBack();
    LIter< T > erase(LIter< T > p);

    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List & h) noexcept;

    void splice(LIter<T> pos, List& h) noexcept;
    void splice(LIter<T> pos, List& h, LIter<T> it) noexcept;
    void splice(LIter<T> pos, List& h, LIter<T> first, LIter<T> last) noexcept;
    void merge(List& h) noexcept;
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
    next_(nullptr),
    prev_(nullptr)
  {
    new (storage_) T(value);
  }

  template< class T >
  detail::Node< T >::Node(T && value):
    next_(nullptr),
    prev_(nullptr)
  {
    new (storage_) T(std::move(value));
  }

  template< class T >
  template< class... Args >
  detail::Node< T >::Node(Args&&... args):
    next_(nullptr),
    prev_(nullptr)
  {
    new (storage_) T(std::forward< Args >(args)...);
  }

  template< class T >
  detail::Node< T >::~Node()
  {
    getValue().~T();
  }

  template< class T >
  T & detail::Node< T >::getValue()
  {
    return *reinterpret_cast< T * >(storage_);
  }

  template< class T >
  const T & detail::Node< T >::getValue() const
  {
    return *reinterpret_cast< const T * >(storage_);
  }

  template< class T >
  LIter< T >::LIter(detail::Node< T > * p):
    ptr_(p)
  {}

  template< class T >
  T & LIter< T >::operator*()
  {
    return ptr_->getValue();
  }

  template< class T >
  T * LIter< T >::operator->()
  {
    return &ptr_->getValue();
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
  bool LIter< T >::operator==(const LIter & h) const
  {
    return ptr_ == h.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter & h) const
  {
    return !(ptr_ == h.ptr_);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T > * p):
    ptr_(p)
  {}

  template< class T >
  const T & LCIter< T >::operator*() const
  {
    return ptr_->getValue();
  }

  template< class T >
  const T * LCIter< T >::operator->() const
  {
    return &ptr_->getValue();
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
  bool LCIter< T >::operator==(const LCIter & h) const
  {
    return ptr_ == h.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter & h) const
  {
    return !(ptr_ == h.ptr_);
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
      pushBack(cur->getValue());
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
    return head_->getValue();
  }

  template< class T >
  const T & List< T >::front() const
  {
    return head_->getValue();
  }

  template< class T >
  T & List< T >::back()
  {
    return tail_->getValue();
  }

  template< class T >
  const T & List< T >::back() const
  {
    return tail_->getValue();
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
  void List< T >::pushFront(T && v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::move(v));
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
  void List< T >::pushBack(T && v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::move(v));
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
  template< class... Args >
  LIter< T > List< T >::emplaceFront(Args&&... args)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< Args >(args)...);
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
    return LIter< T >(newNode);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplaceBack(Args&&... args)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< Args >(args)...);
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
    return LIter< T >(newNode);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplace(LIter< T > p, Args&&... args)
  {
    if (!p.ptr_)
    {
      return emplaceBack(std::forward< Args >(args)...);
    }
    if (p.ptr_ == head_)
    {
      return emplaceFront(std::forward< Args >(args)...);
    }
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< Args >(args)...);
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
  template< class... Args >
  LIter< T > List< T >::emplaceAfter(LIter< T > p, Args&&... args)
  {
    if (!p.ptr_)
    {
      return emplaceBack(std::forward< Args >(args)...);
    }
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< Args >(args)...);
    detail::Node< T > * current = p.ptr_;
    detail::Node< T > * next = current->next_;
    newNode->prev_ = current;
    newNode->next_ = next;
    current->next_ = newNode;
    if (next)
    {
      next->prev_ = newNode;
    }
    else
    {
      tail_ = newNode;
    }
    ++size_;
    return LIter< T >(newNode);
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > p, const T & v)
  {
    return emplace(p, v);
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
  void List< T >::splice(LIter< T > pos, List & h) noexcept
  {
    if (h.size_ == 0 || this == &h)
    {
      return;
    }
    splice(pos, h, h.begin(), h.end());
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List & h, LIter< T > it) noexcept
  {
    if (it.ptr_ == nullptr || h.size_ == 0)
    {
      return;
    }
    LIter< T > next = it;
    ++next;
    splice(pos, h, it, next);
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List & h, LIter< T > first, LIter< T > last) noexcept
  {
    if (first == last || h.size_ == 0)
    {
      return;
    }
    size_t count = 0;
    for (LIter< T > it = first; it != last; ++it)
    {
      ++count;
    }
    detail::Node< T > * firstNode = first.ptr_;
    detail::Node< T > * lastNode = (last.ptr_ == nullptr) ? h.tail_ : last.ptr_->prev_;
    if (firstNode->prev_)
    {
      firstNode->prev_->next_ = last.ptr_;
    }
    else
    {
      h.head_ = last.ptr_;
    }
    if (last.ptr_)
    {
      last.ptr_->prev_ = firstNode->prev_;
    }
    else
    {
      h.tail_ = firstNode->prev_;
    }
    detail::Node< T > * posNode = pos.ptr_;
    detail::Node< T > * posPrev = (posNode) ? posNode->prev_ : tail_;
    firstNode->prev_ = posPrev;
    lastNode->next_ = posNode;
    if (posPrev)
    {
      posPrev->next_ = firstNode;
    }
    else
    {
      head_ = firstNode;
    }
    if (posNode)
    {
      posNode->prev_ = lastNode;
    }
    else
    {
      tail_ = lastNode;
    }
    size_ += count;
    h.size_ -= count;
  }

  template< class T >
  void List< T >::merge(List & h) noexcept
  {
    if (this == &h || h.size_ == 0)
    {
      return;
    }
    LIter< T > thisIt = begin();
    LIter< T > hIt = h.begin();
    while (thisIt != end() && hIt != h.end())
    {
      if (*hIt < *thisIt)
      {
        LIter< T > toMove = hIt;
        ++hIt;
        splice(thisIt, h, toMove);
      }
      else
      {
        ++thisIt;
      }
    }
    if (hIt != h.end())
    {
      splice(end(), h, hIt, h.end());
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
    List< T > falseList;
    LIter< T > it = begin();
    while (it != end())
    {
      if (!p(*it))
      {
        LIter< T > toMove = it;
        ++it;
        falseList.splice(falseList.end(), *this, toMove);
      }
      else
      {
        ++it;
      }
    }
    size_t falseCount = falseList.size();
    if (falseCount > 0)
    {
      splice(end(), falseList);
    }
    if (falseCount == 0)
    {
      return end();
    }
    LIter< T > result = begin();
    for (size_t i = 0; i < size_ - falseCount; ++i)
    {
      ++result;
    }
    return result;
  }
}

#endif

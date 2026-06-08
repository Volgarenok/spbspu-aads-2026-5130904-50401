#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include <cstddef>
#include <limits>
#include <cassert>
#include <functional>
#include <memory>

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
      T val;
      Node< T > * next;
      Node< T > * prev;
      Node(const T & value);
      Node(T && value);

      template< class... Args >
      explicit Node(Args&&... args);
    };
  }

  template< class T >
  class LIter
  {
  public:
    T & operator*() noexcept;
    T * operator->() noexcept;

    LIter & operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter & operator--() noexcept;
    LIter operator--(int) noexcept;

    bool operator==(const LIter & h) const noexcept;
    bool operator!=(const LIter & h) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T > * ptr_;
    explicit LIter(detail::Node< T > * p = nullptr) noexcept;
  };

  template< class T >
  class LCIter
  {
  public:
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;

    LCIter & operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter & operator--() noexcept;
    LCIter operator--(int) noexcept;

    bool operator==(const LCIter & h) const noexcept;
    bool operator!=(const LCIter & h) const noexcept;
  private:
    friend class List< T >;
    const detail::Node< T > * ptr_;
    LCIter(const detail::Node< T > * p = nullptr) noexcept;
  };

  template< class T >
  class List
  {
  public:
    List() noexcept;
    List(const List & h);
    List(List && h) noexcept;
    ~List() noexcept;

    List & operator=(const List & h);
    List & operator=(List && h) noexcept;

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;

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
    LIter< T > insert(LIter< T > p, T && v);

    void popFront() noexcept;
    void popBack() noexcept;
    LIter< T > erase(LIter< T > p) noexcept;

    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List & h) noexcept;

    void splice(LIter< T > pos, List & h) noexcept;
    void splice(LIter< T > pos, List & h, LIter< T > it) noexcept;
    void splice(LIter< T > pos, List & h, LIter< T > first, LIter< T > last) noexcept;
    void merge(List & h) noexcept;
    template< class Comp >
    void merge(List & h, Comp c) noexcept;
    void sort();
    template< class Comp >
    void sort(Comp c);

    template< class P >
    LIter< T > partition(P p);
  private:
    detail::Node< T > * head_;
    detail::Node< T > * tail_;
    size_t size_;

    template< class Comp >
    void mergeSort(List< T > & list, Comp c);

    void insertNodeBeforeHead(detail::Node< T > * newNode) noexcept;
    void insertNodeAfterTail(detail::Node< T > * newNode) noexcept;
  };

  template< class T >
  detail::Node< T >::Node(const T & value):
    val(value),
    next(nullptr),
    prev(nullptr)
  {}

  template< class T >
  detail::Node< T >::Node(T && value):
    val(std::forward< T >(value)),
    next(nullptr),
    prev(nullptr)
  {}

  template< class T >
  template< class... Args >
  detail::Node< T >::Node(Args&&... args) :
    val(std::forward< Args >(args)...),
    next(nullptr),
    prev(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T > * p) noexcept:
    ptr_(p)
  {}

  template< class T >
  T & LIter< T >::operator*() noexcept
  {
    return ptr_->val;
  }

  template< class T >
  T * LIter< T >::operator->() noexcept
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter temp = *this;
    ptr_ = ptr_->next;
    return temp;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--() noexcept
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter temp = *this;
    ptr_ = ptr_->prev;
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter & h) const noexcept
  {
    return ptr_ == h.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter & h) const noexcept
  {
    return !(ptr_ == h.ptr_);
  }

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T > * p) noexcept:
    ptr_(p)
  {}

  template< class T >
  const T & LCIter< T >::operator*() const noexcept
  {
    return ptr_->val;
  }

  template< class T >
  const T * LCIter< T >::operator->() const noexcept
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter tmp = *this;
    ptr_ = ptr_->next;
    return tmp;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--() noexcept
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter tmp = *this;
    ptr_ = ptr_->prev;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter & h) const noexcept
  {
    return ptr_ == h.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter & h) const noexcept
  {
    return !(ptr_ == h.ptr_);
  }

  template< class T >
  List< T >::List() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List & h):
    List()
  {
    for (detail::Node< T > * cur = h.head_; cur != nullptr; cur = cur->next)
    {
      pushBack(cur->val);
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
    assert(this != std::addressof(h));

    List temp(h);
    swap(temp);
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List && h) noexcept
  {
    assert(this != std::addressof(h));
    List temp(std::move(h));
    swap(temp);
    return *this;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  T & List< T >::front()
  {
    return head_->val;
  }

  template< class T >
  const T & List< T >::front() const
  {
    return head_->val;
  }

  template< class T >
  T & List< T >::back()
  {
    return tail_->val;
  }

  template< class T >
  const T & List< T >::back() const
  {
    return tail_->val;
  }

  template< class T >
  void List< T >::insertNodeBeforeHead(detail::Node< T > * newNode) noexcept
  {
    newNode->next = head_;
    if (head_)
    {
      head_->prev = newNode;
    }
    else
    {
      tail_ = newNode;
    }
    head_ = newNode;
    ++size_;
  }

  template< class T >
  void List< T >::insertNodeAfterTail(detail::Node< T > * newNode) noexcept
  {
    newNode->prev = tail_;
    if (tail_)
    {
      tail_->next = newNode;
    }
    else
    {
      head_ = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(const T & v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(v);
    insertNodeBeforeHead(newNode);
  }

  template< class T >
  void List< T >::pushFront(T && v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< T >(v));
    insertNodeBeforeHead(newNode);
  }

  template< class T >
  void List< T >::pushBack(const T & v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(v);
    insertNodeAfterTail(newNode);
  }

  template< class T >
  void List< T >::pushBack(T && v)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< T >(v));
    insertNodeAfterTail(newNode);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplaceFront(Args&&... args)
  {
    detail::Node< T > * newNode = new detail::Node< T >(std::forward< Args >(args)...);
    newNode->next = head_;
    if (head_)
    {
      head_->prev = newNode;
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
    newNode->prev = tail_;
    if (tail_)
    {
      tail_->next = newNode;
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
    detail::Node< T > * prev = next->prev;
    newNode->prev = prev;
    newNode->next = next;
    prev->next = newNode;
    next->prev = newNode;
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
    detail::Node< T > * next = current->next;
    newNode->prev = current;
    newNode->next = next;
    current->next = newNode;
    if (next)
    {
      next->prev = newNode;
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
  LIter< T > List< T >::insert(LIter< T > p, T && v)
  {
    return emplace(p, std::forward< T >(v));
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (head_)
    {
      erase(begin());
    }
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (tail_)
    {
      erase(LIter< T >(tail_));
    }
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > p) noexcept
  {
    if (!p.ptr_)
    {
      return end();
    }
    detail::Node< T > * next = p.ptr_->next;
    detail::Node< T > * prev = p.ptr_->prev;
    if (prev)
    {
      prev->next = next;
    }
    else
    {
      head_ = next;
    }
    if (next)
    {
      next->prev = prev;
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
      head_ = head_->next;
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
    if (h.size_ == 0 || this == std::addressof(h))
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
    if (first == last || h.size_ == 0 || this == std::addressof(h))
    {
      return;
    }
    size_t count = 0;
    for (LIter< T > it = first; it != last; ++it)
    {
      ++count;
    }
    detail::Node< T > * firstNode = first.ptr_;
    detail::Node< T > * lastNode = (last.ptr_ == nullptr) ? h.tail_ : last.ptr_->prev;
    if (firstNode->prev)
    {
      firstNode->prev->next = last.ptr_;
    }
    else
    {
      h.head_ = last.ptr_;
    }
    if (last.ptr_)
    {
      last.ptr_->prev = firstNode->prev;
    }
    else
    {
      h.tail_ = firstNode->prev;
    }
    detail::Node< T > * posNode = pos.ptr_;
    detail::Node< T > * posPrev = (posNode) ? posNode->prev : tail_;
    firstNode->prev = posPrev;
    lastNode->next = posNode;
    if (posPrev)
    {
      posPrev->next = firstNode;
    }
    else
    {
      head_ = firstNode;
    }
    if (posNode)
    {
      posNode->prev = lastNode;
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
    merge(h, std::less< T >());
  }

  template< class T >
  template< class Comp >
  void List< T >::merge(List & h, Comp c) noexcept
  {
    if (this == &h || h.size_ == 0)
    {
      return;
    }
    LIter< T > thisIt = begin();
    LIter< T > hIt = h.begin();
    while (thisIt != end() && hIt != h.end())
    {
      if (c(*hIt, *thisIt))
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
  template< class Comp >
  void List< T >::mergeSort(List< T > & list, Comp c)
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
    mergeSort(list, c);
    mergeSort(right, c);
    list.merge(right, c);
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< class T >
  template< class Comp >
  void List< T >::sort(Comp c)
  {
    if (size_ <= 1)
    {
      return;
    }
    mergeSort(*this, c);
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

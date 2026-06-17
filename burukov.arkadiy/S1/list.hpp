#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <stdexcept>
#include <memory>

namespace burukov
{

  template< class T >
  class List;

  namespace detail
  {

    template< class T >
    struct Node
    {
      T value;
      Node< T > *next;

      Node();
      Node(const T &value, Node< T > *next = nullptr);
      Node(T &&value, Node< T > *next = nullptr);
    };

  }

  template< class T >
  class LIter
  {
  public:
    LIter() noexcept;
    LIter(const LIter &) noexcept = default;
    LIter &operator=(const LIter &) noexcept = default;

    T &operator*();
    T *operator->();

    LIter< T > &operator++() noexcept;
    LIter< T > operator++(int) noexcept;

    bool operator==(const LIter< T > &rhs) const noexcept;
    bool operator!=(const LIter< T > &rhs) const noexcept;

  private:
    detail::Node< T > *ptr_;

    explicit LIter(detail::Node< T > *ptr) noexcept;
    detail::Node< T > *get() const noexcept;

    friend class List< T >;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter(const LCIter &) noexcept = default;
    LCIter &operator=(const LCIter &) noexcept = default;

    const T &operator*() const;
    const T *operator->() const;

    LCIter< T > &operator++() noexcept;
    LCIter< T > operator++(int) noexcept;

    bool operator==(const LCIter< T > &rhs) const noexcept;
    bool operator!=(const LCIter< T > &rhs) const noexcept;

  private:
    const detail::Node< T > *ptr_;

    explicit LCIter(const detail::Node< T > *ptr) noexcept;

    friend class List< T >;
  };

  template< class T >
  class List
  {
  public:
    List();
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    ~List() noexcept;

    List< T > &operator=(const List< T > &other);
    List< T > &operator=(List< T > &&other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T &front();
    const T &front() const;

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;

    void pushFront(const T &value);
    void pushFront(T &&value);
    void pushBack(const T &value);
    void pushBack(T &&value);
    void popFront() noexcept;

    LIter< T > insertAfter(LIter< T > pos, const T &value);
    LIter< T > insertAfter(LIter< T > pos, T &&value);
    LIter< T > eraseAfter(LIter< T > pos) noexcept;

    void clear() noexcept;
    void swap(List< T > &other) noexcept;

    void spliceAfter(LIter< T > pos, List< T > &other) noexcept;
    void spliceAfter(LIter< T > pos, List< T > &other, LIter< T > it) noexcept;
    void spliceAfter(LIter< T > pos, List< T > &other, LIter< T > first, LIter< T > last) noexcept;

    void sort();
    template< class Compare >
    void sort(Compare comp);

    void merge(List< T > &other);
    template< class Compare >
    void merge(List< T > &other, Compare comp);

    template< class Predicate >
    LIter< T > partition(Predicate pred);

  private:
    detail::Node< T > *head_;
    detail::Node< T > *tail_;
    size_t size_;

    void insertNode(detail::Node< T > *after, detail::Node< T > *node);

    detail::Node< T > *splitList(detail::Node< T > *head) noexcept;
    template< class Compare >
    detail::Node< T > *mergeSorted(detail::Node< T > *left, detail::Node< T > *right, Compare comp);
    template< class Compare >
    detail::Node< T > *sortList(detail::Node< T > *head, Compare comp);
    detail::Node< T > *getBefore(LIter< T > it) const;
  };

}

namespace burukov
{

  namespace detail
  {

    template< class T >
    Node< T >::Node():
      value(),
      next(nullptr)
    {}

    template< class T >
    Node< T >::Node(const T &value, Node< T > *next):
      value(value),
      next(next)
    {}

    template< class T >
    Node< T >::Node(T &&value, Node< T > *next):
      value(std::forward< T >(value)),
      next(next)
    {}

  }

  template< class T >
  LIter< T >::LIter() noexcept:
    ptr_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T > *ptr) noexcept:
    ptr_(ptr)
  {}

  template< class T >
  T &LIter< T >::operator*()
  {
    return ptr_->value;
  }

  template< class T >
  T *LIter< T >::operator->()
  {
    return std::addressof(ptr_->value);
  }

  template< class T >
  LIter< T > &LIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > &rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class T >
  detail::Node< T > *LIter< T >::get() const noexcept
  {
    return ptr_;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T > *ptr) noexcept:
    ptr_(ptr)
  {}

  template< class T >
  const T &LCIter< T >::operator*() const
  {
    return ptr_->value;
  }

  template< class T >
  const T *LCIter< T >::operator->() const
  {
    return std::addressof(ptr_->value);
  }

  template< class T >
  LCIter< T > &LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > &rhs) const noexcept
  {
    return ptr_ == rhs.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > &rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class T >
  void List< T >::insertNode(detail::Node< T > *after, detail::Node< T > *node)
  {
    if (after == nullptr)
    {
      node->next = head_;
      head_ = node;
    }
    else
    {
      node->next = after->next;
      after->next = node;
    }

    if (tail_ == after || (after == nullptr && tail_ == nullptr))
    {
      tail_ = node;
    }

    ++size_;
  }

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List< T > &other):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (detail::Node< T > *current = other.head_; current; current = current->next)
      {
        pushBack(current->value);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List< T > &&other) noexcept:
    head_(std::exchange(other.head_, nullptr)),
    tail_(std::exchange(other.tail_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T > &List< T >::operator=(const List< T > &other)
  {
    if (this != std::addressof(other))
    {
      List< T > temp(other);
      swap(temp);
    }

    return *this;
  }

  template< class T >
  List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    if (this != std::addressof(other))
    {
      List< T > temp(std::move(other));
      swap(temp);
    }

    return *this;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  T &List< T >::front()
  {
    if (!head_)
    {
      throw std::logic_error("empty list");
    }

    return head_->value;
  }

  template< class T >
  const T &List< T >::front() const
  {
    if (!head_)
    {
      throw std::logic_error("empty list");
    }

    return head_->value;
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
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr);
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
  void List< T >::pushFront(const T &value)
  {
    insertNode(nullptr, new detail::Node< T >(value));
  }

  template< class T >
  void List< T >::pushFront(T &&value)
  {
    insertNode(nullptr, new detail::Node< T >(std::forward< T >(value)));
  }

  template< class T >
  void List< T >::pushBack(const T &value)
  {
    insertNode(tail_, new detail::Node< T >(value));
  }

  template< class T >
  void List< T >::pushBack(T &&value)
  {
    insertNode(tail_, new detail::Node< T >(std::forward< T >(value)));
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (!head_)
    {
      return;
    }

    detail::Node< T > *temp = head_;
    head_ = head_->next;
    delete temp;
    --size_;

    if (!head_)
    {
      tail_ = nullptr;
    }
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, const T &value)
  {
    if (pos == end())
    {
      return end();
    }

    detail::Node< T > *created = new detail::Node< T >(value);
    insertNode(pos.get(), created);

    return LIter< T >(created);
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, T &&value)
  {
    if (pos == end())
    {
      return end();
    }

    detail::Node< T > *created = new detail::Node< T >(std::forward< T >(value));
    insertNode(pos.get(), created);

    return LIter< T >(created);
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > pos) noexcept
  {
    if (pos == end() || !pos.get()->next)
    {
      return end();
    }

    detail::Node< T > *victim = pos.get()->next;
    pos.get()->next = victim->next;

    if (tail_ == victim)
    {
      tail_ = pos.get();
    }

    delete victim;
    --size_;

    return LIter< T >(pos.get()->next);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (head_)
    {
      detail::Node< T > *tmp = head_;
      head_ = head_->next;
      delete tmp;
    }

    tail_ = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::swap(List< T > &other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List< T > &other) noexcept
  {
    if (other.empty() || this == std::addressof(other))
    {
      return;
    }

    detail::Node< T > *insertPos = (pos == end()) ? tail_ : pos.get();

    if (insertPos == nullptr)
    {
      head_ = other.head_;
      tail_ = other.tail_;
    }
    else
    {
      detail::Node< T > *nextAfter = insertPos->next;
      insertPos->next = other.head_;
      other.tail_->next = nextAfter;

      if (insertPos == tail_)
      {
        tail_ = other.tail_;
      }
    }

    size_ += other.size_;

    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List< T > &other, LIter< T > it) noexcept
  {
    if (it == other.end() || other.empty())
    {
      return;
    }

    detail::Node< T > *beforeNode = it.get();

    if (!beforeNode || !beforeNode->next)
    {
      return;
    }

    detail::Node< T > *nodeToMove = beforeNode->next;

    beforeNode->next = nodeToMove->next;

    if (nodeToMove == other.tail_)
    {
      other.tail_ = beforeNode;
    }

    other.size_--;

    detail::Node< T > *insertPos = (pos == end()) ? tail_ : pos.get();

    if (insertPos == nullptr)
    {
      nodeToMove->next = head_;
      head_ = nodeToMove;

      if (tail_ == nullptr)
      {
        tail_ = nodeToMove;
      }
    }
    else
    {
      nodeToMove->next = insertPos->next;
      insertPos->next = nodeToMove;

      if (insertPos == tail_)
      {
        tail_ = nodeToMove;
      }
    }

    size_++;
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List< T > &other, LIter< T > first, LIter< T > last) noexcept
  {
    if (first == last || other.empty())
    {
      return;
    }

    detail::Node< T > *beforeFirst = first.get();

    if (!beforeFirst || !beforeFirst->next)
    {
      return;
    }

    detail::Node< T > *firstNode = beforeFirst->next;
    detail::Node< T > *lastNode = last.get();

    detail::Node< T > *rangeTail = firstNode;
    size_t count = 1;

    while (rangeTail->next != lastNode)
    {
      rangeTail = rangeTail->next;
      ++count;

      if (!rangeTail)
      {
        return;
      }
    }

    beforeFirst->next = lastNode;

    if (rangeTail == other.tail_)
    {
      other.tail_ = beforeFirst;
    }

    other.size_ -= count;

    detail::Node< T > *insertPos = (pos == end()) ? tail_ : pos.get();

    if (insertPos == nullptr)
    {
      rangeTail->next = head_;
      head_ = firstNode;

      if (tail_ == nullptr)
      {
        tail_ = rangeTail;
      }
    }
    else
    {
      rangeTail->next = insertPos->next;
      insertPos->next = firstNode;

      if (insertPos == tail_)
      {
        tail_ = rangeTail;
      }
    }

    size_ += count;
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    if (size_ < 2)
    {
      return;
    }

    head_ = sortList(head_, comp);
    tail_ = head_;

    while (tail_ && tail_->next)
    {
      tail_ = tail_->next;
    }
  }

  template< class T >
  detail::Node< T > *List< T >::splitList(detail::Node< T > *head) noexcept
  {
    detail::Node< T > *slow = head;
    detail::Node< T > *fast = head->next;

    while (fast && fast->next)
    {
      slow = slow->next;
      fast = fast->next->next;
    }

    detail::Node< T > *second = slow->next;
    slow->next = nullptr;

    return second;
  }

  template< class T >
  template< class Compare >
  detail::Node< T > *List< T >::mergeSorted(detail::Node< T > *left, detail::Node< T > *right, Compare comp)
  {
    detail::Node< T > dummy;
    detail::Node< T > *tail = &dummy;

    while (left && right)
    {
      if (comp(right->value, left->value))
      {
        tail->next = right;
        right = right->next;
      }
      else
      {
        tail->next = left;
        left = left->next;
      }

      tail = tail->next;
    }

    tail->next = left ? left : right;

    return dummy.next;
  }

  template< class T >
  template< class Compare >
  detail::Node< T > *List< T >::sortList(detail::Node< T > *head, Compare comp)
  {
    if (!head || !head->next)
    {
      return head;
    }

    detail::Node< T > *second = splitList(head);
    head = sortList(head, comp);
    second = sortList(second, comp);

    return mergeSorted(head, second, comp);
  }

  template< class T >
  void List< T >::merge(List< T > &other)
  {
    merge(other, std::less< T >());
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List< T > &other, Compare comp)
  {
    if (this == std::addressof(other) || other.empty())
    {
      return;
    }

    head_ = mergeSorted(head_, other.head_, comp);
    tail_ = head_;

    while (tail_ && tail_->next)
    {
      tail_ = tail_->next;
    }

    size_ += other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  template< class Predicate >
  LIter< T > List< T >::partition(Predicate pred)
  {
    if (!head_)
    {
      return end();
    }

    detail::Node< T > *trueHead = nullptr;
    detail::Node< T > *trueTail = nullptr;
    detail::Node< T > *falseHead = nullptr;
    detail::Node< T > *falseTail = nullptr;
    detail::Node< T > *curr = head_;

    while (curr)
    {
      detail::Node< T > *next = curr->next;
      curr->next = nullptr;

      if (pred(curr->value))
      {
        if (!trueHead)
        {
          trueHead = curr;
          trueTail = curr;
        }
        else
        {
          trueTail->next = curr;
          trueTail = curr;
        }
      }
      else
      {
        if (!falseHead)
        {
          falseHead = curr;
          falseTail = curr;
        }
        else
        {
          falseTail->next = curr;
          falseTail = curr;
        }
      }

      curr = next;
    }

    if (trueHead)
    {
      trueTail->next = falseHead;
      head_ = trueHead;
      tail_ = falseTail ? falseTail : trueTail;

      return falseHead ? LIter< T >(falseHead) : begin();
    }

    head_ = falseHead;
    tail_ = falseTail;

    return begin();
  }

  template< class T >
  detail::Node< T > *List< T >::getBefore(LIter< T > it) const
  {
    if (it == begin())
    {
      return nullptr;
    }

    detail::Node< T > *curr = head_;

    while (curr && curr->next != it.get())
    {
      curr = curr->next;
    }

    return curr;
  }

}

#endif

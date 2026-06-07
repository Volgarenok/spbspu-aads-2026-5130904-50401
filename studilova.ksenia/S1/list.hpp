#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>
#include <functional>

namespace studilova
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T data;
      Node* next;
      Node* prev;
    };
  }

  template< class T >
  class LIter;

  template< class T >
  class CLIter;

  template< class T >
  class List
  {
    public:
      List() noexcept;
      List(const List& other);
      List(List&& other) noexcept;
      ~List();

      List& operator=(const List& other);
      List& operator=(List&& other) noexcept;

      void swap(List& other) noexcept;

      bool empty() const noexcept;
      void clear() noexcept;
      size_t size() const noexcept;

      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      void popFront() noexcept;
      void popBack() noexcept;

      void pushFront(const T& value);
      void pushFront(T&& value);
      void pushBack(const T& value);
      void pushBack(T&& value);
      void insert(LIter< T > pos, const T& value);
      void insert(LIter< T > pos, T&& value);

      void splice(LIter< T > pos, List& other) noexcept;
      void splice(LIter< T > pos, List& other, LIter< T > it) noexcept;
      void splice(LIter< T > pos, List& other, LIter< T > first, LIter< T > last) noexcept;

      void sort();
      template< class Cmp >
      void sort(Cmp cmp);

      void merge(List& other);
      template< class Cmp >
      void merge(List& other, Cmp cmp);

      template< class P >
      LIter< T > partition(P pred);

      LIter< T > begin() noexcept;
      LIter< T > end() noexcept;

      CLIter< T > begin() const noexcept;
      CLIter< T > end() const noexcept;

    private:
      detail::Node< T >* head_;
      size_t size_;
  };

  template< class T >
  List< T >::List() noexcept:
    head_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List& other):
    head_(nullptr),
    size_(0)
  {
    try
    {
      detail::Node< T >* curr = other.head_;

      for (size_t i = 0; i < other.size_; ++i)
      {
        pushBack(curr->data);
        curr = curr->next;
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    head_(std::exchange(other.head_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }

    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      clear();

      head_ = std::exchange(other.head_, nullptr);
      size_ = std::exchange(other.size_, 0);
    }

    return *this;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  T& List< T >::front()
  {
    if (empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->data;
  }

  template< class T >
  const T& List< T >::front() const
  {
    if (empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->data;
  }

  template< class T >
  T& List< T >::back()
  {
    if (empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->prev->data;
  }

  template< class T >
  const T& List< T >::back() const
  {
    if (empty())
    {
      throw std::out_of_range("List is empty");
    }
    return head_->prev->data;
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (empty())
    {
      return;
    }

    detail::Node< T >* temp = head_;

    if (size_ == 1)
    {
      delete temp;
      head_ = nullptr;
      size_ = 0;
    } else {
      detail::Node< T >* tail = head_->prev;

      head_ = head_->next;

      head_->prev = tail;
      tail->next = head_;

      delete temp;
      size_--;
    }
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (empty())
    {
      return;
    }

    if (size_ == 1)
    {
      delete head_;
      head_ = nullptr;
      size_ = 0;
    } else {
      detail::Node< T >* tail = head_->prev;
      detail::Node< T >* new_tail = tail->prev;

      new_tail->next = head_;
      head_->prev = new_tail;

      delete tail;
      size_--;
    }
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    detail::Node< T >* node = new detail::Node< T >{value, nullptr, nullptr};

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;

      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), nullptr, nullptr };

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;

      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    detail::Node< T >* node = new detail::Node< T >{value, nullptr, nullptr};

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    }
    size_++;
  }

  template< class T >
  void List< T >::pushBack(T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), nullptr, nullptr };

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    } else {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::insert(LIter< T > pos, const T& value)
  {
    if (pos.node_ == nullptr)
    {
      pushBack(value);
      return;
    }
    if (pos.node_ == head_)
    {
      pushFront(value);
      return;
    }

    detail::Node< T >* curr = pos.node_;
    detail::Node< T >* prev = curr->prev;

    detail::Node< T >* node = new detail::Node< T >{value, curr, prev};

    prev->next = node;
    curr->prev = node;

    ++size_;
  }

  template< class T >
  void List< T >::insert(LIter< T > pos, T&& value)
  {
    if (!pos.node_)
    {
      pushBack(std::move(value));
      return;
    }
    if (pos.node_ == head_)
    {
      pushFront(std::move(value));
      return;
    }

    detail::Node< T >* curr = pos.node_;
    detail::Node< T >* prev = curr->prev;
    detail::Node< T >* node = new detail::Node< T >{ std::move(value), curr, prev };

    prev->next = node;
    curr->prev = node;
    ++size_;
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List& other) noexcept
  {
    if (other.empty() || this == &other)
    {
      return;
    }

    detail::Node< T >* first = other.head_;
    detail::Node< T >* last = other.head_->prev;

    if (empty())
    {
      head_ = first;
    }
    else if (pos.node_ == nullptr)
    {
      detail::Node< T >* tail = head_->prev;

      tail->next = first;
      first->prev = tail;

      last->next = head_;
      head_->prev = last;
    } else {
      detail::Node< T >* curr = pos.node_;
      detail::Node< T >* prev = curr->prev;

      prev->next = first;
      first->prev = prev;

      last->next = curr;
      curr->prev = last;

      if (curr == head_)
      {
        head_ = first;
      }
    }
    size_ += other.size_;
    other.head_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List& other, LIter< T > it) noexcept
  {
    if (other.empty() || it.node_ == nullptr || this == &other)
    {
      return;
    }

    detail::Node< T >* node = it.node_;

    if (other.size_ == 1)
    {
      other.head_ = nullptr;
    } else {
      node->prev->next = node->next;
      node->next->prev = node->prev;

      if (other.head_ == node)
      {
        other.head_ = node->next;
      }
    }
    --other.size_;

    if (empty())
    {
      node->next = node;
      node->prev = node;
      head_ = node;
    }
    else if (pos.node_ == nullptr)
    {
      detail::Node< T >* tail = head_->prev;

      node->next = head_;
      node->prev = tail;

      tail->next = node;
      head_->prev = node;
    } else {
      detail::Node< T >* curr = pos.node_;
      detail::Node< T >* prev = curr->prev;

      node->next = curr;
      node->prev = prev;

      prev->next = node;
      curr->prev = node;

      if (curr == head_)
      {
        head_ = node;
      }
    }
    ++size_;
  }

  template< class T >
  void List< T >::splice(LIter< T > pos, List& other, LIter< T > first, LIter< T > last) noexcept
  {
    if (other.empty() || first.node_ == last.node_ || first.node_ == nullptr || this == &other)
    {
      return;
    }

    detail::Node< T >* rangeFirst = first.node_;
    detail::Node< T >* rangeLast = first.node_;
    size_t count = 1;

    while (rangeLast->next != other.head_ && rangeLast->next != last.node_)
    {
      rangeLast = rangeLast->next;
      ++count;
    }

    detail::Node< T >* beforeRange = rangeFirst->prev;
    detail::Node< T >* afterRange = rangeLast->next;

    beforeRange->next = afterRange;
    afterRange->prev = beforeRange;

    if (other.head_ == rangeFirst)
    {
      other.head_ = afterRange;
    }

    other.size_ -= count;

    if (other.size_ == 0)
    {
      other.head_ = nullptr;
    }

    if (empty())
    {
      rangeFirst->prev = rangeLast;
      rangeLast->next = rangeFirst;
      head_ = rangeFirst;
    }
    else if (pos.node_ == nullptr)
    {
      detail::Node< T >* tail = head_->prev;

      tail->next = rangeFirst;
      rangeFirst->prev = tail;

      rangeLast->next = head_;
      head_->prev = rangeLast;
    } else {
      detail::Node< T >* curr = pos.node_;
      detail::Node< T >* prev = curr->prev;

      prev->next = rangeFirst;
      rangeFirst->prev = prev;

      rangeLast->next = curr;
      curr->prev = rangeLast;

      if (curr == head_)
      {
        head_ = rangeFirst;
      }
    }
    size_ += count;
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >{});
  }

  template< class T >
  template< class Cmp >
  void List< T >::sort(Cmp cmp)
  {
    if (size_ <= 1)
    {
      return;
    }

    List< T > second;

    size_t half = size_ / 2;
    LIter< T > mid = begin();

    for (size_t i = 0; i < half; ++i)
    {
      ++mid;
    }

    second.splice(second.end(), *this, mid, end());

    sort(cmp);
    second.sort(cmp);

    merge(second, cmp);
  }

  template< class T >
  void List< T >::merge(List& other)
  {
    merge(other, std::less< T >{});
  }

  template< class T >
  template< class Cmp >
  void List< T >::merge(List& other, Cmp cmp)
  {
    if (this == &other || other.empty())
    {
      return;
    }

    LIter< T > pos = begin();
    size_t posIndex = 0;

    while (!other.empty())
    {
      LIter< T > otherIt = other.begin();

      while (posIndex < size_ && !cmp(*otherIt, *pos))
      {
        ++pos;
        ++posIndex;
      }

      if (posIndex == size_)
      {
        splice(end(), other, otherIt);
      } else {
        splice(pos, other, otherIt);
        ++posIndex;
      }
    }
  }

  template< class T >
  template< class P >
  LIter< T > List< T >::partition(P pred)
  {
    List< T > secondList;

    size_t count = size_;
    LIter< T > it = begin();

    for (size_t i = 0; i < count;)
    {
      LIter< T > current = it;
      ++it;
      ++i;

      if (!pred(*current))
      {
        secondList.splice(secondList.end(), *this, current);
      }
    }

    LIter< T > border = end();

    if (!secondList.empty())
    {
      border = secondList.begin();
      splice(end(), secondList);
    }

    return border;
  }

  template< class T >
  class LIter
  {
    public:
      T& operator*() const;
      T* operator->() const;

      LIter& operator++();
      LIter operator++(int);
      LIter& operator--();
      LIter operator--(int);

      bool operator==(const LIter& other) const;
      bool operator!=(const LIter& other) const;

    private:
      detail::Node< T >* node_;

      explicit LIter(detail::Node< T >* node = nullptr);

      friend class List< T >;
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T >* node):
    node_(node)
  {}

  template< class T >
  T& LIter< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::out_of_range("Iterator dereference error");
    }
    return node_->data;
  }

  template< class T >
  T* LIter< T >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  LIter< T >& LIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter< T > temp(*this);
    --(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const
  {
    return !(*this == other);
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
  class CLIter
  {
    public:
      const T& operator*() const;
      const T* operator->() const;

      CLIter& operator++();
      CLIter operator++(int);
      CLIter& operator--();
      CLIter operator--(int);

      bool operator==(const CLIter& other) const;
      bool operator!=(const CLIter& other) const;

    private:
      detail::Node< T >* node_;

      explicit CLIter(detail::Node< T >* node = nullptr);

      friend class List< T >;
  };

  template< class T >
  CLIter< T >::CLIter(detail::Node< T >* node):
    node_(node)
  {}

  template< class T >
  const T& CLIter< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::out_of_range("Iterator dereference error");
    }
    return node_->data;
  }

  template< class T >
  const T* CLIter< T >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class T >
  CLIter< T >& CLIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  CLIter< T > CLIter< T >::operator++(int)
  {
    CLIter< T > temp(*this);
    ++(*this);
    return temp;
  }

  template< class T >
  CLIter< T >& CLIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  CLIter< T > CLIter< T >::operator--(int)
  {
    CLIter< T > temp(*this);
    --(*this);
    return temp;
  }

  template< class T >
  bool CLIter< T >::operator==(const CLIter& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool CLIter< T >::operator!=(const CLIter& other) const
  {
    return !(*this == other);
  }

  template< class T >
  CLIter< T > List< T >::begin() const noexcept
  {
    return CLIter< T >(head_);
  }

  template< class T >
  CLIter< T > List< T >::end() const noexcept
  {
    return CLIter< T >(nullptr);
  }
}

#endif

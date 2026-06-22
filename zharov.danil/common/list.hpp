#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T val;
      Node< T >* next;
      Node< T >* prev;

      template< class... Args >
      Node(Args&&... args):
        val(std::forward< Args >(args)...),
        next(nullptr),
        prev(nullptr)
      {}
    };
  }
  template< class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    T& operator*() noexcept;
    T* operator->() noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;
    bool operator==(const LIter& it) const noexcept;
    bool operator!=(const LIter& it) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;
    LIter(detail::Node< T >* node) noexcept;
  };

  template< class T >
  class LCIter
  {
  public:
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;
    bool operator==(const LCIter& it) const noexcept;
    bool operator!=(const LCIter& it) const noexcept;

  private:
    friend class List< T >;
    const detail::Node< T >* curr_;
    LCIter(const detail::Node< T >* node) noexcept;
  };

  template< class T >
  class List
  {
  public:
    List() noexcept;
    List(const List& h);
    List(List&& h) noexcept;
    ~List() noexcept;
    List& operator=(const List& h);
    List& operator=(List&& h) noexcept;
    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;
    void pushFront(const T& v);
    void pushBack(const T& v);
    LIter< T > insert(LIter< T > pos, const T& v);
    void pushFront(T&& v);
    void pushBack(T&& v);
    LIter< T > insert(LIter< T > pos, T&& v);
    void popFront() noexcept;
    void popBack() noexcept;
    LIter< T > erase(LIter< T > pos) noexcept;
    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List< T >& h) noexcept;
    void splice(LIter< T > pos, List< T >& other) noexcept;
    void splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept;
    void splice(LIter< T > pos, List< T >& other, LIter< T > first, LIter< T > last) noexcept;
    template< class Compare >
    void sort(Compare comp);
    void sort();
    template< class Compare >
    void merge(List< T >& other, Compare comp);
    void merge(List< T >& other);
    template< class Predicate >
    LIter< T > partition(Predicate pred);
    template< class... Args >
    LIter< T > emplaceFront(Args&&... args);
    template< class... Args >
    LIter< T > emplaceBack(Args&&... args);
    template< class... Args >
    LIter< T > emplace(LIter< T > pos, Args&&... args);

  private:
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    size_t size_;
    void spliceRange(detail::Node< T >* first,
      detail::Node< T >* last,
      size_t count,
      List< T >& src,
      detail::Node< T >* pos) noexcept;
  };
}

template< class T >
zharov::LIter< T >::LIter(detail::Node< T >* node) noexcept:
  curr_(node)
{}

template< class T >
T& zharov::LIter< T >::operator*() noexcept
{
  return curr_->val;
}

template< class T >
T* zharov::LIter< T >::operator->() noexcept
{
  return std::addressof(curr_->val);
}

template< class T >
zharov::LIter< T >& zharov::LIter< T >::operator++() noexcept
{
  curr_ = curr_->next;
  return *this;
}

template< class T >
zharov::LIter< T > zharov::LIter< T >::operator++(int) noexcept
{
  LIter< T > temp = *this;
  curr_ = curr_->next;
  return temp;
}

template< class T >
zharov::LIter< T >& zharov::LIter< T >::operator--() noexcept
{
  curr_ = curr_->prev;
  return *this;
}

template< class T >
zharov::LIter< T > zharov::LIter< T >::operator--(int) noexcept
{
  LIter< T > temp = *this;
  curr_ = curr_->prev;
  return temp;
}

template< class T >
bool zharov::LIter< T >::operator==(const LIter& it) const noexcept
{
  return curr_ == it.curr_;
}

template< class T >
bool zharov::LIter< T >::operator!=(const LIter& it) const noexcept
{
  return curr_ != it.curr_;
}

template< class T >
zharov::LCIter< T >::LCIter(const detail::Node< T >* node) noexcept:
  curr_(node)
{}

template< class T >
const T& zharov::LCIter< T >::operator*() const noexcept
{
  return curr_->val;
}

template< class T >
const T* zharov::LCIter< T >::operator->() const noexcept
{
  return std::addressof(curr_->val);
}

template< class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator++() noexcept
{
  curr_ = curr_->next;
  return *this;
}

template< class T >
zharov::LCIter< T > zharov::LCIter< T >::operator++(int) noexcept
{
  LCIter< T > temp = *this;
  curr_ = curr_->next;
  return temp;
}

template< class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator--() noexcept
{
  curr_ = curr_->prev;
  return *this;
}

template< class T >
zharov::LCIter< T > zharov::LCIter< T >::operator--(int) noexcept
{
  LCIter< T > temp = *this;
  curr_ = curr_->prev;
  return temp;
}

template< class T >
bool zharov::LCIter< T >::operator==(const LCIter& it) const noexcept
{
  return curr_ == it.curr_;
}

template< class T >
bool zharov::LCIter< T >::operator!=(const LCIter& it) const noexcept
{
  return curr_ != it.curr_;
}

template< class T >
zharov::List< T >::List() noexcept:
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template< class T >
zharov::List< T >::List(const List< T >& h):
  List()
{
  for (detail::Node< T >* curr = h.head_; curr != nullptr; curr = curr->next)
  {
    pushBack(curr->val);
  }
}

template< class T >
zharov::List< T >::List(List< T >&& h) noexcept:
  head_(std::exchange(h.head_, nullptr)),
  tail_(std::exchange(h.tail_, nullptr)),
  size_(std::exchange(h.size_, 0))
{}

template< class T >
zharov::List< T >::~List() noexcept
{
  clear();
}

template< class T >
void zharov::List< T >::swap(List< T >& h) noexcept
{
  std::swap(h.head_, head_);
  std::swap(h.tail_, tail_);
  std::swap(h.size_, size_);
}

template< class T >
zharov::List< T >& zharov::List< T >::operator=(const List< T >& h)
{
  if (this != std::addressof(h))
  {
    List< T > temp(h);
    swap(temp);
  }
  return *this;
}

template< class T >
zharov::List< T >& zharov::List< T >::operator=(List< T >&& h) noexcept
{
  if (this != std::addressof(h))
  {
    List tmp(std::move(h));
    swap(tmp);
  }
  return *this;
}

template< class T >
zharov::LIter< T > zharov::List< T >::begin() noexcept
{
  return LIter< T >(head_);
}

template< class T >
zharov::LIter< T > zharov::List< T >::end() noexcept
{
  return LIter< T >(nullptr);
}

template< class T >
zharov::LCIter< T > zharov::List< T >::cbegin() const noexcept
{
  return LCIter< T >(head_);
}

template< class T >
zharov::LCIter< T > zharov::List< T >::cend() const noexcept
{
  return LCIter< T >(nullptr);
}

template< class T >
T& zharov::List< T >::front() noexcept
{
  return head_->val;
}

template< class T >
const T& zharov::List< T >::front() const noexcept
{
  return head_->val;
}

template< class T >
T& zharov::List< T >::back() noexcept
{
  return tail_->val;
}

template< class T >
const T& zharov::List< T >::back() const noexcept
{
  return tail_->val;
}

template< class T >
template< class... Args >
zharov::LIter< T > zharov::List< T >::emplace(LIter< T > pos, Args&&... args)
{
  detail::Node< T >* node = new detail::Node< T >(std::forward< Args >(args)...);
  if (!pos.curr_)
  {
    node->prev = tail_;
    if (tail_)
    {
      tail_->next = node;
    }
    else
    {
      head_ = node;
    }
    tail_ = node;
    ++size_;
    return LIter< T >(tail_);
  }
  if (pos.curr_ == head_)
  {
    node->next = head_;
    head_->prev = node;
    head_ = node;
    ++size_;
    return LIter< T >(head_);
  }
  detail::Node< T >* next = pos.curr_;
  detail::Node< T >* prev = next->prev;
  node->next = next;
  node->prev = prev;
  prev->next = node;
  next->prev = node;
  ++size_;
  return LIter< T >(node);
}

template< class T >
template< class... Args >
zharov::LIter< T > zharov::List< T >::emplaceFront(Args&&... args)
{
  return emplace(begin(), std::forward< Args >(args)...);
}

template< class T >
template< class... Args >
zharov::LIter< T > zharov::List< T >::emplaceBack(Args&&... args)
{
  return emplace(end(), std::forward< Args >(args)...);
}

template< class T >
void zharov::List< T >::pushFront(const T& v)
{
  emplaceFront(v);
}

template< class T >
void zharov::List< T >::pushFront(T&& v)
{
  emplaceFront(std::forward< T >(v));
}

template< class T >
void zharov::List< T >::pushBack(const T& v)
{
  emplaceBack(v);
}

template< class T >
void zharov::List< T >::pushBack(T&& v)
{
  emplaceBack(std::forward< T >(v));
}

template< class T >
zharov::LIter< T > zharov::List< T >::insert(LIter< T > pos, const T& v)
{
  return emplace(pos, v);
}

template< class T >
zharov::LIter< T > zharov::List< T >::insert(LIter< T > pos, T&& v)
{
  return emplace(pos, std::forward< T >(v));
}

template< class T >
void zharov::List< T >::popFront() noexcept
{
  erase(begin());
}

template< class T >
void zharov::List< T >::popBack() noexcept
{
  erase(LIter< T >(tail_));
}

template< class T >
zharov::LIter< T > zharov::List< T >::erase(LIter< T > pos) noexcept
{
  if (!pos.curr_)
  {
    return end();
  }
  detail::Node< T >* next = pos.curr_->next;
  detail::Node< T >* prev = pos.curr_->prev;
  if (next)
  {
    next->prev = prev;
  }
  else
  {
    tail_ = prev;
  }
  if (prev)
  {
    prev->next = next;
  }
  else
  {
    head_ = next;
  }
  delete pos.curr_;
  --size_;
  return LIter< T >(next);
}

template< class T >
void zharov::List< T >::clear() noexcept
{
  while (head_)
  {
    detail::Node< T >* next = head_->next;
    delete head_;
    head_ = next;
    --size_;
  }
  tail_ = nullptr;
}

template< class T >
size_t zharov::List< T >::size() const noexcept
{
  return size_;
}

template< class T >
void zharov::List< T >::spliceRange(detail::Node< T >* first,
  detail::Node< T >* last,
  size_t count,
  List< T >& src,
  detail::Node< T >* pos) noexcept
{
  detail::Node< T >* before_first = first->prev;
  detail::Node< T >* after_last = last->next;
  if (before_first)
  {
    before_first->next = after_last;
  }
  else
  {
    src.head_ = after_last;
  }
  if (after_last)
  {
    after_last->prev = before_first;
  }
  else
  {
    src.tail_ = before_first;
  }
  src.size_ -= count;
  first->prev = (pos != nullptr) ? pos->prev : tail_;
  last->next = pos;
  if (first->prev)
  {
    first->prev->next = first;
  }
  else
  {
    head_ = first;
  }
  if (last->next)
  {
    last->next->prev = last;
  }
  else
  {
    tail_ = last;
  }
  size_ += count;
}

template< class T >
void zharov::List< T >::splice(LIter< T > pos, List< T >& other) noexcept
{
  if (!other.head_)
  {
    return;
  }
  spliceRange(other.head_, other.tail_, other.size_, other, pos.curr_);
}

template< class T >
void zharov::List< T >::splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept
{
  spliceRange(it.curr_, it.curr_, 1, other, pos.curr_);
}

template< class T >
void zharov::List< T >::splice(LIter< T > pos,
  List< T >& other,
  LIter< T > first,
  LIter< T > last) noexcept
{
  if (first.curr_ == last.curr_)
  {
    return;
  }
  detail::Node< T >* last_node = (last.curr_ != nullptr) ? last.curr_->prev : other.tail_;
  size_t count = 0;
  for (detail::Node< T >* curr = first.curr_; curr != last.curr_; curr = curr->next)
  {
    ++count;
  }
  spliceRange(first.curr_, last_node, count, other, pos.curr_);
}

template< class T >
template< class Compare >
void zharov::List< T >::sort(Compare comp)
{
  LIter< T > sorted_end = begin();
  while (sorted_end != end())
  {
    LIter< T > min_it = sorted_end;
    for (LIter< T > it = sorted_end; it != end(); ++it)
    {
      if (comp(*it, *min_it))
      {
        min_it = it;
      }
    }
    if (min_it != sorted_end)
    {
      splice(sorted_end, *this, min_it);
    }
    else
    {
      ++sorted_end;
    }
  }
}

template< class T >
void zharov::List< T >::sort()
{
  sort(std::less< T >{});
}

template< class T >
template< class Compare >
void zharov::List< T >::merge(List< T >& other, Compare comp)
{
  splice(end(), other);
  sort(comp);
}

template< class T >
void zharov::List< T >::merge(List< T >& other)
{
  merge(other, std::less< T >{});
}

template< class T >
template< class Predicate >
zharov::LIter< T > zharov::List< T >::partition(Predicate pred)
{
  List< T > yes;
  List< T > no;
  while (head_)
  {
    detail::Node< T >* curr = head_;
    if (pred(curr->val))
    {
      yes.spliceRange(curr, curr, 1, *this, nullptr);
    }
    else
    {
      no.spliceRange(curr, curr, 1, *this, nullptr);
    }
  }
  LIter< T > partition_point(no.head_);
  splice(end(), yes);
  splice(end(), no);
  return partition_point;
}

#endif

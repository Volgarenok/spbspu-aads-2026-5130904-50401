#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace kuznetsov {
  namespace detail {
    template< class T >
    struct Node {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }

  template< class T >
  class List;

  template< class T >
  class LCIter {
  public:
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;

    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const LCIter& y) const noexcept;
    bool operator!=(const LCIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;

    LCIter(detail::Node< T >* pn) noexcept;
  };

  template< class T >
  class LIter {
  public:
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;

    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    bool operator==(const LIter& y) const noexcept;
    bool operator!=(const LIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;

    LIter(detail::Node< T >* pn) noexcept;
  };

  template< class T >
  class LRCIter {
  public:
    LRCIter& operator++() noexcept;

    LRCIter operator++(int) noexcept;

    LRCIter& operator--() noexcept;

    LRCIter operator--(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const LRCIter& y) const noexcept;

    bool operator!=(const LRCIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;

    LRCIter(detail::Node< T >* pn) noexcept;
  };

  template< class T >
  class LRIter {
  public:
    LRIter& operator++() noexcept;

    LRIter operator++(int) noexcept;

    LRIter& operator--() noexcept;

    LRIter operator--(int) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    bool operator==(const LRIter& y) const noexcept;

    bool operator!=(const LRIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;

    LRIter(detail::Node< T >* pn) noexcept;
  };

  template< class T >
  class List {
  public:
    List() noexcept;

    List(const List& other);

    List(List&& other) noexcept;

    ~List() noexcept;

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    LIter< T > insert(LCIter< T > it, T&& val);
    LIter< T > insert(LCIter< T > it, const T& val);

    T& front();

    T& back();

    const T& front() const;

    const T& back() const;

    void popFront();

    void popBack();

    LIter< T > begin() noexcept;

    LIter< T > end() noexcept;

    LCIter< T > cbegin() const noexcept;

    LCIter< T > cend() const noexcept;

    LRIter< T > rbegin() noexcept;

    LRIter< T > rend() noexcept;

    LRCIter< T > rcbegin() const noexcept;

    LRCIter< T > rcend() const noexcept;

    void clear() noexcept;

    LIter< T > erase(LCIter< T > it);

    size_t size() const noexcept;
    void swap(List&) noexcept;
    bool empty() const noexcept;

    void splice(LCIter< T > pos, List& other) noexcept;
    void splice(LCIter< T > pos, List& other, LCIter< T > it) noexcept;
    void splice(LCIter< T > pos, List& other, LCIter< T > first, LCIter< T > last) noexcept;

    void splice(LCIter< T > pos, List&& other) noexcept;
    void splice(LCIter< T > pos, List&& other, LCIter< T > it) noexcept;
    void splice(LCIter< T > pos, List&& other, LCIter< T > first, LCIter< T > last) noexcept;

    template< class Compare >
    void sort(Compare cmp) noexcept;

    template< class Compare >
    void merge(List& other, Compare cmp) noexcept;

    template< class Compare >
    void merge(List&& other, Compare cmp) noexcept;

    template< class Predict >
    LIter< T > partition(Predict pred) noexcept;

  private:
    using node_t = detail::Node< T >;
    node_t* head_;
    size_t size_;

    void attachList(node_t* pos, node_t* first, node_t* last, size_t count) noexcept;
    void detachList(node_t* first, node_t* last, size_t count) noexcept;
    void move(node_t* pos, List& src, node_t* first, node_t* last, size_t count) noexcept;
  };

}

template< class T >
kuznetsov::List< T >::List() noexcept:
  head_(nullptr),
  size_(0)
{}

template< class T >
kuznetsov::List< T >::List(const List& other):
  head_(nullptr),
  size_(0)
{
  if (other.empty()) {
    return;
  }
  detail::Node< T >* current = other.head_;
  do {
    try {
      insert(cend(), current->val);
      current = current->next;
    } catch (...) {
      clear();
      throw;
    }
  } while (current != other.head_);
}

template< class T >
kuznetsov::List< T >::List(List&& other) noexcept:
  head_(std::exchange(other.head_, nullptr)),
  size_(std::exchange(other.size_, 0))
{}

template< class T >
kuznetsov::List< T >::~List() noexcept
{
  clear();
}

template< class T >
kuznetsov::List< T >& kuznetsov::List< T >::operator=(const List& other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  List cp(other);
  swap(cp);

  return *this;
}

template< class T >
kuznetsov::List< T >& kuznetsov::List< T >::operator=(List&& other) noexcept
{
  if (this == std::addressof(other)) {
    return *this;
  }
  clear();
  head_ = std::exchange(other.head_, nullptr);
  size_ = std::exchange(other.size_, 0);

  return *this;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::insert(LCIter< T > it, T&& val)
{
  detail::Node< T >* n = new detail::Node< T >{ T(std::forward< T >(val)), nullptr, nullptr };
  attachList(it.curr_, n, n, 1);
  return LIter< T >(n);
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::insert(LCIter< T > it, const T& val)
{
  detail::Node< T >* n = new detail::Node< T >{ T(val), nullptr, nullptr };
  attachList(it.curr_, n, n, 1);
  return LIter< T >(n);
}

template< class T >
T& kuznetsov::List< T >::front()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->val;
}

template< class T >
T& kuznetsov::List< T >::back()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->prev->val;
}

template< class T >
const T& kuznetsov::List< T >::front() const
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->val;
}

template< class T >
const T& kuznetsov::List< T >::back() const
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->prev->val;
}

template< class T >
void kuznetsov::List< T >::popFront()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  erase(cbegin());
}

template< class T >
void kuznetsov::List< T >::popBack()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  erase(LCIter< T >(head_->prev));
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::begin() noexcept
{
  return LIter< T >(head_);
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::end() noexcept
{
  return LIter< T >(nullptr);
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::List< T >::cbegin() const noexcept
{
  return LCIter< T >(head_);
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::List< T >::cend() const noexcept
{
  return LCIter< T >(nullptr);
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::List< T >::rbegin() noexcept
{
  assert(head_ != nullptr);
  return LRIter< T >(head_->prev);
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::List< T >::rend() noexcept
{
  return LRIter< T >(nullptr);
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::List< T >::rcbegin() const noexcept
{
  assert(head_ != nullptr);
  return LRCIter< T >(head_->prev);
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::List< T >::rcend() const noexcept
{
  return LRCIter< T >(nullptr);
}

template< class T >
void kuznetsov::List< T >::clear() noexcept
{
  if (!head_) {
    return;
  }
  detail::Node< T >* curr = head_;
  do {
    detail::Node< T >* next = curr->next;
    delete curr;
    curr = next;
  } while (curr != head_);
  size_ = 0;
  head_ = nullptr;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::erase(LCIter< T > it)
{
  if (!head_ || !it.curr_) {
    throw std::logic_error("Empty list or iterator");
  }
  detail::Node< T >* nextNode = it.curr_->next;
  detachList(it.curr_, it.curr_, 1);
  delete it.curr_;
  if (!size_) {
    return LIter< T >(nullptr);
  }
  return LIter< T >(nextNode);
}

template< class T >
size_t kuznetsov::List< T >::size() const noexcept
{
  return size_;
}

template< class T >
bool kuznetsov::List< T >::empty() const noexcept
{
  return !size_;
}

template< class T >
void kuznetsov::List< T >::swap(List& oth) noexcept
{
  std::swap(oth.head_, head_);
  std::swap(oth.size_, size_);
}

template< class T >
void kuznetsov::List< T >::attachList(node_t* pos, node_t* first, node_t* last, size_t count) noexcept
{
  if (count == 0) {
    return;
  }
  if (head_ == nullptr) {
    first->prev = last;
    last->next = first;
    head_ = first;
    size_ = count;
    return;
  }
  node_t* anch = pos ? pos : head_;
  node_t* prevAnch = anch->prev;
  prevAnch->next = first;
  first->prev = prevAnch;
  anch->prev = last;
  last->next = anch;
  if (pos == head_) {
    head_ = first;
  }
  size_ += count;
}

template< class T >
void kuznetsov::List< T >::detachList(node_t* first, node_t* last, size_t count) noexcept
{
  if (!size_) {
    return;
  }
  if (count == size_) {
    head_ = nullptr;
    size_ = 0;
    return;
  }
  node_t* prevFirst = first->prev;
  node_t* afterLast = last->next;
  prevFirst->next = afterLast;
  afterLast->prev = prevFirst;
  node_t* p = first;
  while (true) {
    if (p == head_) {
      head_ = afterLast;
      break;
    } else if (p == last) {
      break;
    }
    p = p->next;
  }
  size_ -= count;
}

template< class T >
void kuznetsov::List< T >::move(node_t* pos, List& src, node_t* first, node_t* last, size_t count) noexcept
{
  if (count == 0) {
    return;
  }
  src.detachList(first, last, count);
  this->attachList(pos, first, last, count);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List& other) noexcept
{
  if (std::addressof(other) == this || other.empty()) {
    return;
  }
  node_t* first = other.head_;
  node_t* last = other.head_->prev;
  move(pos.curr_, other, first, last, other.size_);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List& other, LCIter< T > it) noexcept
{
  if (it.curr_ == nullptr) {
    return;
  }
  if (std::addressof(other) == this && pos.curr_ == it.curr_) {
    return;
  }
  move(pos.curr_, other, it.curr_, it.curr_, 1);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List& other, LCIter< T > first, LCIter< T > last) noexcept
{
  if (first == last || other.empty()) {
    return;
  }
  node_t* fst = first.curr_;
  node_t* lst = last.curr_ ? last.curr_->prev : other.head_->prev;
  size_t cnt = 0;
  node_t* p = fst;
  while (true) {
    ++cnt;
    if (p == lst) {
      break;
    }
    p = p->next;
  }
  move(pos.curr_, other, fst, lst, cnt);
}

template< class T >
template< class Compare >
void kuznetsov::List< T >::merge(List& other, Compare cmp) noexcept
{
  if (std::addressof(other) == this || other.empty()) {
    return;
  }
  if (empty()) {
    swap(other);
    return;
  }
  node_t* i = head_;
  size_t processed = 0;
  size_t origSize = size_;
  while (processed < origSize && !other.empty()) {
    if (cmp(other.head_->val, i->val)) {
      node_t* node = other.head_;
      move(i, other, node, node, 1);
    } else {
      i = i->next;
      ++processed;
    }
  }
  if (!other.empty()) {
    node_t* oFirst = other.head_;
    node_t* oLast = other.head_->prev;
    size_t cnt = other.size_;
    move(nullptr, other, oFirst, oLast, cnt);
  }
}

template< class T >
template< class Compare >
void kuznetsov::List< T >::sort(Compare cmp) noexcept
{
  if (size_ <= 1) {
    return;
  }
  size_t half = size_ / 2;
  node_t* mid = head_;
  for (size_t i = 0; i < half; ++i) {
    mid = mid->next;
  }
  List other;
  node_t* tail = head_->prev;
  other.move(nullptr, *this, mid, tail, size_ - half);
  this->sort(cmp);
  other.sort(cmp);
  this->merge(other, cmp);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List&& other) noexcept
{
  splice(pos, other);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List&& other, LCIter< T > it) noexcept
{
  splice(pos, other, it);
}

template< class T >
void kuznetsov::List< T >::splice(LCIter< T > pos, List&& other, LCIter< T > first, LCIter< T > last) noexcept
{
  splice(pos, other, first, last);
}

template< class T >
template< class Compare >
void kuznetsov::List< T >::merge(List&& other, Compare cmp) noexcept
{
  merge(other, cmp);
}

template< class T >
template< class Predict >
kuznetsov::LIter< T > kuznetsov::List< T >::partition(Predict pred) noexcept
{
  if (empty()) {
    return LIter< T >(nullptr);
  }
  if (size_ == 1) {
    if (pred(head_->val)) {
      return LIter< T >(nullptr);
    }
    return LIter< T >(head_);
  }
  List rejected;
  node_t* i = head_;
  size_t remaining = size_;
  while (remaining > 0) {
    node_t* next = i->next;
    if (!pred(i->val)) {
      rejected.move(nullptr, *this, i, i, 1);
    }
    i = next;
    --remaining;
  }
  node_t* firstRejected = rejected.head_;
  if (!rejected.empty()) {
    move(nullptr, rejected, rejected.head_, rejected.head_->prev, rejected.size_);
  }
  return LIter< T >(firstRejected);
}

template< class T >
kuznetsov::LCIter< T >::LCIter(detail::Node< T >* pn) noexcept:
  curr_(pn)
{}

template< class T >
kuznetsov::LCIter< T >& kuznetsov::LCIter< T >::operator++() noexcept
{
  if (curr_) {
    curr_ = curr_->next;
  }
  return *this;
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::LCIter< T >::operator++(int) noexcept
{
  LCIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LCIter< T >& kuznetsov::LCIter< T >::operator--() noexcept
{
  if (curr_) {
    curr_ = curr_->prev;
  }
  return *this;
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::LCIter< T >::operator--(int) noexcept
{
  LCIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
const T& kuznetsov::LCIter< T >::operator*() const noexcept
{
  assert(this->curr_);
  return curr_->val;
}

template< class T >
const T* kuznetsov::LCIter< T >::operator->() const noexcept
{
  assert(this->curr_);
  return std::addressof(curr_->val);
}

template< class T >
bool kuznetsov::LCIter< T >::operator==(const LCIter& y) const noexcept
{
  return this->curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LCIter< T >::operator!=(const LCIter& y) const noexcept
{
  return !(*this == y);
}

template< class T >
kuznetsov::LIter< T >::LIter(kuznetsov::detail::Node< T >* pn) noexcept:
  curr_(pn)
{}

template< class T >
kuznetsov::LIter< T >& kuznetsov::LIter< T >::operator++() noexcept
{
  if (curr_) {
    curr_ = curr_->next;
  }
  return *this;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::LIter< T >::operator++(int) noexcept
{
  LIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LIter< T >& kuznetsov::LIter< T >::operator--() noexcept
{
  if (curr_) {
    curr_ = curr_->prev;
  }
  return *this;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::LIter< T >::operator--(int) noexcept
{
  LIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
T& kuznetsov::LIter< T >::operator*() noexcept
{
  assert(this->curr_);
  return curr_->val;
}

template< class T >
T* kuznetsov::LIter< T >::operator->() noexcept
{
  assert(this->curr_);
  return std::addressof(curr_->val);
}

template< class T >
bool kuznetsov::LIter< T >::operator==(const LIter& y) const noexcept
{
  return this->curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LIter< T >::operator!=(const LIter& y) const noexcept
{
  return !(*this == y);
}

template< class T >
kuznetsov::LRCIter< T >::LRCIter(detail::Node< T >* pn) noexcept:
  curr_(pn)
{}

template< class T >
kuznetsov::LRCIter< T >& kuznetsov::LRCIter< T >::operator++() noexcept
{
  if (curr_) {
    curr_ = curr_->prev;
  }
  return *this;
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::LRCIter< T >::operator++(int) noexcept
{
  LRCIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LRCIter< T >& kuznetsov::LRCIter< T >::operator--() noexcept
{
  if (curr_) {
    curr_ = curr_->next;
  }
  return *this;
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::LRCIter< T >::operator--(int) noexcept
{
  LRCIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
const T& kuznetsov::LRCIter< T >::operator*() const noexcept
{
  assert(this->curr_);
  return curr_->val;
}

template< class T >
const T* kuznetsov::LRCIter< T >::operator->() const noexcept
{
  assert(this->curr_);
  return std::addressof(curr_->val);
}

template< class T >
bool kuznetsov::LRCIter< T >::operator==(const LRCIter& y) const noexcept
{
  return curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LRCIter< T >::operator!=(const LRCIter& y) const noexcept
{
  return !(*this == y);
}

template< class T >
kuznetsov::LRIter< T >::LRIter(detail::Node< T >* pn) noexcept:
  curr_(pn)
{}

template< class T >
kuznetsov::LRIter< T >& kuznetsov::LRIter< T >::operator++() noexcept
{
  if (curr_) {
    curr_ = curr_->prev;
  }
  return *this;
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::LRIter< T >::operator++(int) noexcept
{
  LRIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LRIter< T >& kuznetsov::LRIter< T >::operator--() noexcept
{
  if (curr_) {
    curr_ = curr_->next;
  }
  return *this;
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::LRIter< T >::operator--(int) noexcept
{
  LRIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
T& kuznetsov::LRIter< T >::operator*() noexcept
{
  assert(this->curr_);
  return curr_->val;
}

template< class T >
T* kuznetsov::LRIter< T >::operator->() noexcept
{
  assert(this->curr_);
  return std::addressof(curr_->val);
}

template< class T >
bool kuznetsov::LRIter< T >::operator==(const LRIter& y) const noexcept
{
  return curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LRIter< T >::operator!=(const LRIter& y) const noexcept
{
  return !(*this == y);
}

#endif

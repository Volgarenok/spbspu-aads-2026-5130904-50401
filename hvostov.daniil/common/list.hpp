#ifndef LIST_HPP
#define LIST_HPP

#include <functional>
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

namespace hvostov {
  namespace detail {
    template< class T >
    struct Node {
      T val;
      Node< T >* next;
    };
  }

  template< class T >
  class List;

  template< class T >
  class Liter {
  public:
    Liter() = default;
    Liter(const Liter< T >& liter) = default;
    Liter(Liter< T >&& liter) noexcept = default;
    Liter< T >& operator=(const Liter< T >& liter) = default;
    Liter< T >& operator=(Liter< T >&& liter) noexcept = default;
    Liter< T >& operator++() noexcept;
    Liter< T > operator++(int);
    T& operator*() noexcept;
    T* operator->() noexcept;
    bool operator==(const Liter< T >& liter) const noexcept;
    bool operator!=(const Liter< T >& liter) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;
    Liter(detail::Node< T >* n) noexcept;
  };

  template< class T >
  class LCiter {
  public:
    LCiter() = default;
    LCiter(const LCiter< T >& lciter) = default;
    LCiter(LCiter< T >&& lciter) noexcept = default;
    LCiter< T >& operator=(const LCiter< T >& lciter) = default;
    LCiter< T >& operator=(LCiter< T >&& lciter) noexcept = default;
    LCiter< T >& operator++() noexcept;
    LCiter< T > operator++(int);
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator==(const LCiter< T >& lciter) const noexcept;
    bool operator!=(const LCiter< T >& lciter) const noexcept;

  private:
    friend class List< T >;
    const detail::Node< T >* curr_;
    LCiter(const detail::Node< T >* n) noexcept;
  };

  template< class T >
  class List {
  public:
    List();
    List(const List< T >& list);
    List(List< T >&& list) noexcept;
    ~List();
    List< T >& operator=(const List< T >& list);
    List< T >& operator=(List< T >&& list) noexcept;

    Liter< T > begin() noexcept;
    Liter< T > end() noexcept;
    LCiter< T > begin() const noexcept;
    LCiter< T > end() const noexcept;
    LCiter< T > cend() const noexcept;
    LCiter< T > cbegin() const noexcept;

    Liter< T > insertAfter(const Liter< T > it, const T& val);
    Liter< T > insertAfter(const Liter< T > it, T&& val);
    void pushFront(const T& val);
    void pushFront(T&& val);

    template< class... Args >
    Liter< T > emplace(const Liter< T > it, Args&&... args);
    template< class... Args >
    void emplaceFront(Args&&... args);

    void eraseAfter(const Liter< T > it);
    void swap(List< T >& list) noexcept;
    void clear();
    bool empty() const noexcept;
    size_t size() const noexcept;

    void splice(LCiter< T > pos, List< T >& other) noexcept;
    void splice(LCiter< T > pos, List< T >&& other) noexcept;
    void splice(LCiter< T > pos, List< T >& other, LCiter< T > it) noexcept;
    void splice(LCiter< T > pos, List< T >&& other, LCiter< T > it) noexcept;
    void splice(LCiter< T > pos, List< T >& other, LCiter< T > first, LCiter< T > last) noexcept;
    void splice(LCiter< T > pos, List< T >&& other, LCiter< T > first, LCiter< T > last) noexcept;

    void sort();
    template< class Compare >
    void sort(Compare comp);

    void merge(List< T >& other);
    void merge(List< T >&& other);
    template< class Compare >
    void merge(List< T >& other, Compare comp);
    template< class Compare >
    void merge(List< T >&& other, Compare comp);

    template< class Cond >
    void partition(Cond pred);

  private:
    detail::Node< T >* fake_;
    size_t size_;
    detail::Node< T >* createFake();
    void rmFake() noexcept;
  };
}

template< class T >
template< class... Args >
hvostov::Liter< T > hvostov::List< T >::emplace(const Liter< T > it, Args&&... args)
{
  detail::Node< T >* n = new detail::Node< T >{T{std::forward< Args >(args)...}, it.curr_->next};
  it.curr_->next = n;
  size_++;
  return {n};
}

template< class T >
template< class... Args >
void hvostov::List< T >::emplaceFront(Args&&... args)
{
  emplace(Liter< T >(fake_), std::forward< Args >(args)...);
}

template< class T >
hvostov::detail::Node< T >* hvostov::List< T >::createFake()
{
  return new detail::Node< T >();
}

template< class T >
void hvostov::List< T >::rmFake() noexcept
{
  if (fake_) {
    delete fake_;
    fake_ = nullptr;
  }
}

template< class T >
hvostov::Liter< T >::Liter(detail::Node< T >* n) noexcept:
  curr_(n)
{}

template< class T >
hvostov::LCiter< T >::LCiter(const detail::Node< T >* n) noexcept:
  curr_(n)
{}

template< class T >
void hvostov::List< T >::swap(List< T >& list) noexcept
{
  std::swap(fake_, list.fake_);
  std::swap(size_, list.size_);
}

template< class T >
hvostov::LCiter< T > hvostov::List< T >::begin() const noexcept
{
  if (fake_ == nullptr) {
    return {nullptr};
  }
  return {fake_->next};
}

template< class T >
hvostov::LCiter< T > hvostov::List< T >::end() const noexcept
{
  if (fake_ == nullptr) {
    return {nullptr};
  }
  return {fake_};
}

template< class T >
hvostov::List< T >::List():
  fake_(createFake()),
  size_(0)
{
  fake_->next = fake_;
}

template< class T >
hvostov::List< T >::List(const List< T >& list):
  fake_(createFake()),
  size_(0)
{
  fake_->next = fake_;
  Liter< T > mit = begin();
  for (LCiter< T > it = list.begin(); it != list.end(); it++) {
    try {
      mit = insertAfter(mit, *it);
    } catch (...) {
      clear();
      rmFake();
      throw;
    }
  }
}

template< class T >
hvostov::List< T >::List(List< T >&& list) noexcept:
  fake_(std::exchange(list.fake_, nullptr)),
  size_(std::exchange(list.size_, 0))
{}

template< class T >
hvostov::List< T >::~List()
{
  clear();
  rmFake();
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(const List< T >& list)
{
  assert(this != std::addressof(list) && "self-assignment detected");
  List< T > cpy(list);
  swap(cpy);
  return *this;
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(List< T >&& list) noexcept
{
  assert(this != std::addressof(list) && "self-assignment detected");
  clear();
  rmFake();
  List< T > temp(std::move(list));
  swap(temp);
  return *this;
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::begin() noexcept
{
  return {fake_->next};
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::end() noexcept
{
  return {fake_};
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::insertAfter(const Liter< T > it, const T& val)
{
  return emplace(it, val);
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::insertAfter(const Liter< T > it, T&& val)
{
  return emplace(it, std::forward< T >(val));
}

template< class T >
void hvostov::List< T >::pushFront(const T& val)
{
  emplaceFront(val);
}

template< class T >
void hvostov::List< T >::pushFront(T&& val)
{
  emplaceFront(std::forward< T >(val));
}

template< class T >
void hvostov::List< T >::eraseAfter(const Liter< T > it)
{
  if (it.curr_->next != fake_) {
    detail::Node< T >* d = it.curr_->next;
    it.curr_->next = d->next;
    size_--;
    delete d;
  }
}

template< class T >
void hvostov::List< T >::clear()
{
  if (fake_ == nullptr) {
    return;
  }
  detail::Node< T >* h = fake_->next;
  while (h != fake_) {
    detail::Node< T >* temp = h->next;
    delete h;
    h = temp;
  }
  size_ = 0;
  fake_->next = fake_;
}

template< class T >
size_t hvostov::List< T >::size() const noexcept
{
  if (fake_ == nullptr) {
    return 0;
  }
  return size_;
}

template< class T >
bool hvostov::List< T >::empty() const noexcept
{
  if (fake_ == nullptr) {
    return true;
  }
  return fake_->next == fake_;
}

template< class T >
hvostov::LCiter< T > hvostov::List< T >::cend() const noexcept
{
  if (fake_ == nullptr) {
    return {nullptr};
  }
  return {fake_};
}

template< class T >
hvostov::LCiter< T > hvostov::List< T >::cbegin() const noexcept
{
  if (fake_ == nullptr) {
    return {nullptr};
  }
  return {fake_->next};
}

template< class T >
hvostov::Liter< T >& hvostov::Liter< T >::operator++() noexcept
{
  curr_ = curr_->next;
  return *this;
}

template< class T >
hvostov::Liter< T > hvostov::Liter< T >::operator++(int)
{
  Liter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
T& hvostov::Liter< T >::operator*() noexcept
{
  return curr_->val;
}

template< class T >
T* hvostov::Liter< T >::operator->() noexcept
{
  return std::addressof(curr_->val);
}

template< class T >
bool hvostov::Liter< T >::operator==(const Liter< T >& liter) const noexcept
{
  return curr_ == liter.curr_;
}

template< class T >
bool hvostov::Liter< T >::operator!=(const Liter< T >& liter) const noexcept
{
  return curr_ != liter.curr_;
}

template< class T >
hvostov::LCiter< T >& hvostov::LCiter< T >::operator++() noexcept
{
  curr_ = curr_->next;
  return *this;
}

template< class T >
hvostov::LCiter< T > hvostov::LCiter< T >::operator++(int)
{
  LCiter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
const T& hvostov::LCiter< T >::operator*() const noexcept
{
  return curr_->val;
}

template< class T >
const T* hvostov::LCiter< T >::operator->() const noexcept
{
  return std::addressof(curr_->val);
}

template< class T >
bool hvostov::LCiter< T >::operator==(const LCiter< T >& lciter) const noexcept
{
  return curr_ == lciter.curr_;
}

template< class T >
bool hvostov::LCiter< T >::operator!=(const LCiter< T >& lciter) const noexcept
{
  return curr_ != lciter.curr_;
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >& other) noexcept
{
  if (other.empty() || this == &other) {
    return;
  }
  detail::Node< T >* other_first = other.fake_->next;
  detail::Node< T >* other_last = other.fake_;
  while (other_last->next != other.fake_) {
    other_last = other_last->next;
  }
  detail::Node< T >* pos_node = const_cast< detail::Node< T >* >(pos.curr_);
  detail::Node< T >* before_pos = fake_;
  if (pos_node == fake_->next) {
    before_pos = fake_;
  } else {
    before_pos = fake_->next;
    while (before_pos->next != pos_node) {
      before_pos = before_pos->next;
    }
  }
  other_last->next = pos_node;
  before_pos->next = other_first;
  size_ += other.size_;
  other.fake_->next = other.fake_;
  other.size_ = 0;
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >&& other) noexcept
{
  splice(pos, other);
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >& other, LCiter< T > it) noexcept
{
  if (this == &other || other.empty()) {
    return;
  }
  detail::Node< T >* node_to_move = const_cast< detail::Node< T >* >(it.curr_->next);
  if (node_to_move == other.fake_) {
    return;
  }
  const_cast< detail::Node< T >* >(it.curr_)->next = node_to_move->next;
  other.size_--;
  detail::Node< T >* pos_node = const_cast< detail::Node< T >* >(pos.curr_);
  detail::Node< T >* before_pos = fake_;
  if (pos_node == fake_->next) {
    before_pos = fake_;
  } else {
    before_pos = fake_->next;
    while (before_pos->next != pos_node) {
      before_pos = before_pos->next;
    }
  }
  node_to_move->next = pos_node;
  before_pos->next = node_to_move;
  size_++;
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >&& other, LCiter< T > it) noexcept
{
  splice(pos, other, it);
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >& other, LCiter< T > first, LCiter< T > last) noexcept
{
  if (this == &other || other.empty() || first.curr_ == last.curr_) {
    return;
  }
  detail::Node< T >* before_first = other.fake_;
  if (first.curr_ != other.fake_->next) {
    before_first = other.fake_->next;
    while (before_first->next != const_cast< detail::Node< T >* >(first.curr_)) {
      before_first = before_first->next;
    }
  }
  size_t count = 0;
  detail::Node< T >* temp = const_cast< detail::Node< T >* >(first.curr_);
  while (temp != const_cast< detail::Node< T >* >(last.curr_) && temp != other.fake_) {
    temp = temp->next;
    count++;
  }
  if (count == 0) {
    return;
  }
  detail::Node< T >* range_last = const_cast< detail::Node< T >* >(first.curr_);
  for (size_t i = 1; i < count; ++i) {
    range_last = range_last->next;
  }
  before_first->next = const_cast< detail::Node< T >* >(last.curr_);
  other.size_ -= count;
  detail::Node< T >* pos_node = const_cast< detail::Node< T >* >(pos.curr_);
  detail::Node< T >* before_pos = fake_;
  if (pos_node == fake_->next) {
    before_pos = fake_;
  } else {
    before_pos = fake_->next;
    while (before_pos->next != pos_node) {
      before_pos = before_pos->next;
    }
  }
  range_last->next = pos_node;
  before_pos->next = const_cast< detail::Node< T >* >(first.curr_);
  size_ += count;
}

template< class T >
void hvostov::List< T >::splice(LCiter< T > pos, List< T >&& other, LCiter< T > first, LCiter< T > last) noexcept
{
  splice(pos, other, first, last);
}

template< class T >
void hvostov::List< T >::sort()
{
  sort(std::less< T >());
}

template< class T >
template< class Compare >
void hvostov::List< T >::sort(Compare comp)
{
  if (size_ <= 1) {
    return;
  }

  detail::Node< T >* slow = fake_->next;
  detail::Node< T >* fast = fake_->next->next;
  while (fast != fake_ && fast->next != fake_) {
    slow = slow->next;
    fast = fast->next->next;
  }

  List< T > right;
  right.fake_->next = slow->next;
  detail::Node< T >* right_end = right.fake_->next;
  while (right_end->next != fake_) {
    right_end = right_end->next;
  }
  right_end->next = right.fake_;
  right.size_ = size_ / 2;

  slow->next = fake_;
  size_ = (size_ + 1) / 2;

  sort(comp);
  right.sort(comp);
  merge(right, comp);
}

template< class T >
template< class Compare >
void hvostov::List< T >::merge(List< T >& other, Compare comp)
{
  if (this == &other || other.empty()) {
    return;
  }
  if (empty()) {
    swap(other);
    return;
  }

  detail::Node< T >* cur = fake_;
  detail::Node< T >* l = fake_->next;
  detail::Node< T >* r = other.fake_->next;
  size_t merged_size = 0;

  while (l != fake_ && r != other.fake_) {
    if (comp(l->val, r->val)) {
      cur->next = l;
      l = l->next;
    } else {
      cur->next = r;
      r = r->next;
    }
    cur = cur->next;
    merged_size++;
  }

  while (l != fake_) {
    cur->next = l;
    l = l->next;
    cur = cur->next;
    merged_size++;
  }

  while (r != other.fake_) {
    cur->next = r;
    r = r->next;
    cur = cur->next;
    merged_size++;
  }

  cur->next = fake_;
  size_ = merged_size;
  other.fake_->next = other.fake_;
  other.size_ = 0;
}

template< class T >
void hvostov::List< T >::merge(List< T >& other)
{
  merge(other, std::less< T >());
}

template< class T >
void hvostov::List< T >::merge(List< T >&& other)
{
  merge(other, std::less< T >());
}

template< class T >
template< class Compare >
void hvostov::List< T >::merge(List< T >&& other, Compare comp)
{
  merge(other, comp);
}

template< class T >
template< class Cond >
void hvostov::List< T >::partition(Cond pred)
{
  if (size_ <= 1) {
    return;
  }
  List< T > true_list;
  List< T > false_list;
  detail::Node< T >* curr = fake_->next;
  while (curr != fake_) {
    detail::Node< T >* next = curr->next;
    if (pred(curr->val)) {
      curr->next = true_list.fake_;
      if (true_list.empty()) {
        true_list.fake_->next = curr;
      } else {
        detail::Node< T >* last = true_list.fake_->next;
        while (last->next != true_list.fake_) {
          last = last->next;
        }
        last->next = curr;
      }
      true_list.size_++;
    } else {
      curr->next = false_list.fake_;
      if (false_list.empty()) {
        false_list.fake_->next = curr;
      } else {
        detail::Node< T >* last = false_list.fake_->next;
        while (last->next != false_list.fake_) {
          last = last->next;
        }
        last->next = curr;
      }
      false_list.size_++;
    }
    curr = next;
  }
  fake_->next = fake_;
  size_ = 0;
  if (!true_list.empty()) {
    detail::Node< T >* true_last = true_list.fake_->next;
    while (true_last->next != true_list.fake_) {
      true_last = true_last->next;
    }
    fake_->next = true_list.fake_->next;
    true_last->next = fake_;
    size_ += true_list.size_;
    if (!false_list.empty()) {
      detail::Node< T >* false_last = false_list.fake_->next;
      while (false_last->next != false_list.fake_) {
        false_last = false_last->next;
      }
      true_last->next = false_list.fake_->next;
      false_last->next = fake_;
      size_ += false_list.size_;
    }
  } else if (!false_list.empty()) {
    detail::Node< T >* false_last = false_list.fake_->next;
    while (false_last->next != false_list.fake_) {
      false_last = false_last->next;
    }
    fake_->next = false_list.fake_->next;
    false_last->next = fake_;
    size_ += false_list.size_;
  }
  true_list.fake_->next = true_list.fake_;
  true_list.size_ = 0;
  false_list.fake_->next = false_list.fake_;
  false_list.size_ = 0;
}

#endif

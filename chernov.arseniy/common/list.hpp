#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <functional>
#include <utility>

#include <liter.hpp>
#include <lciter.hpp>

namespace chernov {
  namespace detail {
    template< class T >
    struct Node {
      T data;
      Node< T > * next;
    };
  }

  template< class T >
  class List {
  public:
    List();
    List(const List< T > & list);
    List(List< T > && list) noexcept;
    ~List() noexcept;
    List< T > & operator=(const List< T > & list);
    List< T > & operator=(List< T > && list) noexcept;
    T & first();
    const T & first() const;
    LIter< T > beforeBegin() const noexcept;
    LCIter< T > cbeforeBegin() const noexcept;
    LIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    template< class U >
    LIter< T > insertAfter(LIter< T > pos, U && value);
    LIter< T > eraseAfter(LIter< T > pos);
    LIter< T > eraseAfter(LIter< T > first, LIter< T > last);
    void pushFront(const T & value);
    void pushFront(T && value);
    void popFront();
    void swap(List< T > & other) noexcept;
    void spliceAfter(LIter< T > pos, List< T > & other);
    void spliceAfter(LIter< T > pos, List< T > && other);
    void spliceAfter(LIter< T > pos, List< T > & other, LIter< T > it);
    void spliceAfter(LIter< T > pos, List< T > && other, LIter< T > it);
    void spliceAfter(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last);
    void spliceAfter(LIter< T > pos, List< T > && other, LIter< T > first, LIter< T > last);

    void sort();

    template< class Compare >
    void sort(Compare comp);

    void merge(List< T > & other);

    template < class Compare >
    void merge(List< T > & other, Compare comp);

    template< class UnaryPredicate >
    LIter< T > partition(LIter< T > first, LIter< T > last, UnaryPredicate pred);
  private:
    detail::Node< T > * fake_;
    size_t size_;
    detail::Node< T > * createFake();
    void removeFake() noexcept;
  };

  template< class T >
  detail::Node< T > * List< T >::createFake()
  {
    fake_ = new detail::Node< T >();
    return fake_;
  }

  template< class T >
  void List< T >::removeFake() noexcept
  {
    delete fake_;
  }

  template< class T >
  List< T >::List():
    fake_(createFake()),
    size_(0)
  {
    fake_->next = fake_;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    removeFake();
  }

  template< class T >
  List< T >::List(const List< T > & list):
    fake_(nullptr),
    size_(0)
  {
    createFake();
    fake_->next = fake_;
    if (!list.empty()) {
      LIter< T > pos = beforeBegin();
      LIter< T > iter = list.begin();
      do {
        pos = insertAfter(pos, *iter);
        ++iter;
      } while (iter != list.begin());
    }
  }

  template< class T >
  List< T >::List(List< T > && list) noexcept:
    fake_(std::exchange(list.fake_, nullptr)),
    size_(std::exchange(list.size_, 0))
  {}

  template< class T >
  List< T > & List< T >::operator=(const List< T > & list)
  {
    if (this == std::addressof(list)) {
      return *this;
    }
    List< T > new_list = list;
    swap(new_list);
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List< T > && other) noexcept
  {
    if (this != std::addressof(other)) {
      swap(other);
    }
    return *this;
  }

  template< class T >
  T & List< T >::first()
  {
    return fake_->next->data;
  }

  template< class T >
  const T & List< T >::first() const
  {
    return fake_->next->data;
  }

  template< class T >
  LIter< T > List< T >::beforeBegin() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cbeforeBegin() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LIter< T > List< T >::begin() const noexcept
  {
    return {fake_->next, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return {fake_->next, fake_};
  }

  template< class T >
  LIter< T > List< T >::end() const noexcept
  {
    return {fake_, fake_};
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return {fake_, fake_};
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
  void List< T >::clear() noexcept
  {
    if (fake_ == nullptr) {
      return;
    }
    detail::Node< T > * node = fake_->next;
    while (node != fake_) {
      detail::Node< T > * next = node->next;
      delete node;
      node = next;
    }
    fake_->next = fake_;
    size_ = 0;
  }

  template< class T >
  template< class U >
  LIter< T > List< T >::insertAfter(LIter< T > pos, U && value)
  {
    detail::Node< T > * node = new detail::Node< T >{std::forward< U >(value), pos.ptr_->next};
    pos.ptr_->next = node;
    ++size_;
    return {node, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > pos)
  {
    if (pos.ptr_ == nullptr || (pos.ptr_ == fake_ && fake_->next == fake_)) {
      return end();
    }
    detail::Node< T > * del_node = pos.ptr_->next;
    if (del_node == fake_) {
      pos.ptr_ = fake_;
      del_node = fake_->next;
      if (del_node == fake_) {
        return end();
      }
    }
    pos.ptr_->next = del_node->next;
    delete del_node;
    --size_;
    return {pos.ptr_->next, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > first, LIter< T > last)
  {
    if (first == last) {
      return last;
    }
    detail::Node< T > * prev = first.ptr_;
    detail::Node< T > * curr = prev->next;
    bool crossed_fake = false;
    while (curr != last.ptr_) {
      if (curr == fake_) {
        crossed_fake = true;
        curr = fake_->next;
        if (curr == last.ptr_) {
          break;
        }
      }
      detail::Node< T > * next = curr->next;
      delete curr;
      --size_;
      curr = next;
    }
    prev->next = last.ptr_;
    if (crossed_fake) {
      last.ptr_->next = fake_;
      fake_->next = first.ptr_;
    }
    return last;
  }

  template< class T >
  void List< T >::pushFront(const T & value)
  {
    insertAfter(beforeBegin(), value);
  }

  template< class T >
  void List< T >::pushFront(T && value)
  {
    insertAfter(beforeBegin(), std::move(value));
  }

  template< class T >
  void List< T >::popFront()
  {
    eraseAfter(beforeBegin());
  }

  template< class T >
  void List< T >::swap(List< T > & other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > & other)
{
  if (other.empty()) {
    return;
  }

  detail::Node< T > * other_first = other.fake_->next;
  detail::Node< T > * other_last = other_first;
  while (other_last->next != other.fake_) {
    other_last = other_last->next;
  }

  detail::Node< T > * after_pos = pos.ptr_->next;
  pos.ptr_->next = other_first;
  other_last->next = after_pos;

  size_ += other.size_;
  other.size_ = 0;
  other.fake_->next = other.fake_;
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > && other)
{
  spliceAfter(pos, other);
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > & other, LIter< T > it)
{
  if (other.empty() || it.ptr_->next == other.fake_) {
    return;
  }

  detail::Node< T > * first_ptr = it.ptr_;
  detail::Node< T > * last_ptr = it.ptr_->next->next;

  LIter< T > first{first_ptr, other.fake_};
  LIter< T > last{last_ptr, other.fake_};

  spliceAfter(pos, other, first, last);
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > && other, LIter< T > it)
{
  spliceAfter(pos, other, it);
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > & other, LIter< T > first, LIter< T > last)
{
  if (first == last) {
    return;
  }

  detail::Node< T > * other_next = first.ptr_->next;
  if (other_next == last.ptr_) {
    return;
  }

  detail::Node< T > * prev = first.ptr_;
  detail::Node< T > * curr = other_next;
  size_t count = 0;

  while (curr != last.ptr_) {
    prev = curr;
    curr = curr->next;
    ++count;

    if (curr == other.fake_ && last.ptr_ != other.fake_) {
      return;
    }
  }

  detail::Node< T > * curr_next = pos.ptr_->next;

  pos.ptr_->next = other_next;
  prev->next = curr_next;

  first.ptr_->next = last.ptr_;

  size_ += count;
  other.size_ -= count;
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > && other, LIter< T > first, LIter< T > last)
{
  spliceAfter(pos, other, first, last);
}

template< class T >
void chernov::List< T >::sort()
{
  sort(std::less< T >{});
}

template< class T >
template< class Compare >
void chernov::List< T >::sort(Compare comp)
{
  List< T > temp;
  while (!empty()) {
    detail::Node< T > * node = fake_->next;
    fake_->next = node->next;
    --size_;

    detail::Node< T > * prev = temp.fake_;
    detail::Node< T > * curr = prev->next;
    while (curr != temp.fake_ && comp(curr->data, node->data)) {
      prev = curr;
      curr = curr->next;
    }

    node->next = curr;
    prev->next = node;
    ++temp.size_;
  }
  swap(temp);
}

template< class T >
void chernov::List< T >::merge(List< T > & other)
{
  merge(other, std::less< T >{});
}

template< class T >
template< class Compare >
void chernov::List< T >::merge(List< T > & other, Compare comp)
{
  if (other.empty()) {
    return;
  }
  if (empty()) {
    swap(other);
    return;
  }

  detail::Node< T > * tail = fake_;
  detail::Node< T > * cur1 = fake_->next;
  detail::Node< T > * prev2 = other.fake_;
  detail::Node< T > * cur2 = other.fake_->next;

  while (cur1 != fake_ && cur2 != other.fake_) {
    if (comp(cur2->data, cur1->data)) {
      prev2->next = cur2->next;
      cur2->next = cur1;
      tail->next = cur2;
      tail = cur2;
      cur2 = prev2->next;
    } else {
      tail = cur1;
      cur1 = cur1->next;
    }
  }

  if (cur2 != other.fake_) {
    tail->next = cur2;
    detail::Node< T > * last = cur2;
    while (last->next != other.fake_) {
      last = last->next;
    }
    last->next = fake_;
  }

  size_ += other.size_;
  other.size_ = 0;
  other.fake_->next = other.fake_;
}

template< class T >
template< class UnaryPredicate >
chernov::LIter< T > chernov::List< T >::partition(LIter< T > first, LIter< T > last, UnaryPredicate pred)
{
  if (first == last) {
    return first;
  }

  detail::Node< T > * prev_first = fake_;
  if (fake_->next != first.ptr_) {
    detail::Node< T > * curr = fake_->next;
    while (curr != fake_ && curr->next != first.ptr_) {
      curr = curr->next;
    }
    prev_first = curr;
  }

  detail::Node< T > * head_true = nullptr;
  detail::Node< T > * tail_true = nullptr;
  detail::Node< T > * head_false = nullptr;
  detail::Node< T > * tail_false = nullptr;

  detail::Node< T > * curr = first.ptr_;
  while (curr != last.ptr_) {
    detail::Node< T > * next_node = curr->next;
    if (pred(curr->data)) {
      if (!head_true) {
        head_true = curr;
        tail_true = curr;
      } else {
        tail_true->next = curr;
        tail_true = curr;
      }
    } else {
      if (!head_false) {
        head_false = curr;
        tail_false = curr;
      } else {
        tail_false->next = curr;
        tail_false = curr;
      }
    }
    curr = next_node;
  }

  if (head_true) {
    prev_first->next = head_true;
    tail_true->next = (head_false ? head_false : last.ptr_);
  } else {
    prev_first->next = (head_false ? head_false : last.ptr_);
  }

  if (head_false) {
    tail_false->next = last.ptr_;
  }

  return (head_false ? LIter< T >(head_false, fake_) : last);
}

#endif

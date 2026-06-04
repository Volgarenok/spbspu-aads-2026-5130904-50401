#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
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
    LIter< T > insertAfter(LIter< T > pos, const T & value);
    LIter< T > insertAfter(LIter< T > pos, T && value);
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
    clear();
    if (!list.empty()) {
      LIter< T > pos = beforeBegin();
      LIter< T > iter = list.begin();
      do {
        pos = insertAfter(pos, *iter);
        ++iter;
      } while (iter != list.begin());
    }
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
  LIter< T > List< T >::insertAfter(LIter< T > pos, const T & value)
  {
    detail::Node< T > * node = new detail::Node< T >{value, pos.ptr->next};
    pos.ptr->next = node;
    ++size_;
    return {node, fake_};
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, T && value)
  {
    detail::Node< T > * node = new detail::Node< T >{std::move(value), pos.ptr->next};
    pos.ptr->next = node;
    ++size_;
    return {node, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > pos)
  {
    if (pos.ptr == nullptr || (pos.ptr == fake_ && fake_->next == fake_)) {
      return end();
    }
    detail::Node< T > * del_node = pos.ptr->next;
    if (del_node == fake_) {
      pos.ptr = fake_;
      del_node = fake_->next;
      if (del_node == fake_) {
        return end();
      }
    }
    pos.ptr->next = del_node->next;
    delete del_node;
    --size_;
    return {pos.ptr->next, fake_};
  }

  template< class T >
  LIter< T > List< T >::eraseAfter(LIter< T > first, LIter< T > last)
  {
    if (first == last) {
      return last;
    }
    detail::Node< T > * prev = first.ptr;
    detail::Node< T > * curr = prev->next;
    bool crossed_fake = false;
    while (curr != last.ptr) {
      if (curr == fake_) {
        crossed_fake = true;
        curr = fake_->next;
        if (curr == last.ptr) {
          break;
        }
      }
      detail::Node< T > * next = curr->next;
      delete curr;
      --size_;
      curr = next;
    }
    prev->next = last.ptr;
    if (crossed_fake) {
      last.ptr->next = fake_;
      fake_->next = first.ptr;
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

  detail::Node< T > * after_pos = pos.ptr->next;
  pos.ptr->next = other_first;
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
  if (other.empty() || it.ptr->next == other.fake_) {
    return;
  }

  detail::Node< T > * first_ptr = it.ptr;
  detail::Node< T > * last_ptr = it.ptr->next->next;

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

  detail::Node< T > * other_next = first.ptr->next;
  if (other_next == last.ptr) {
    return;
  }

  detail::Node< T > * prev = first.ptr;
  detail::Node< T > * curr = other_next;
  size_t count = 0;

  while (curr != last.ptr) {
    prev = curr;
    curr = curr->next;
    ++count;

    if (curr == other.fake_ && last.ptr != other.fake_) {
      return;
    }
  }

  detail::Node< T > * curr_next = pos.ptr->next;

  pos.ptr->next = other_next;
  prev->next = curr_next;

  first.ptr->next = last.ptr;

  size_ += count;
  other.size_ -= count;
}

template< class T >
void chernov::List< T >::spliceAfter(LIter< T > pos, List< T > && other, LIter< T > first, LIter< T > last)
{
  spliceAfter(pos, other, first, last);
}

#endif

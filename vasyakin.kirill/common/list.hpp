#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <limits>
#include <memory>
#include <functional>
#include <type_traits>
#include <new>

namespace vasyakin
{
  namespace detail
  {
    template< class T > class Node;
    struct FakeTag {};
  }

  template< class T > class List;
  const size_t max = std::numeric_limits< size_t >::max();

  template< class T >
  class LIter
  {
  public:
    LIter() noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* ptr_;
    explicit LIter(detail::Node< T >* p) noexcept;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashIter;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashConstIter;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashTable;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter() noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;

  private:
    friend class List< T >;
    const detail::Node< T >* ptr_;
    explicit LCIter(const detail::Node< T >* p) noexcept;
    explicit LCIter(const LIter< T >& it) noexcept;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashIter;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashConstIter;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashTable;
  };

  namespace detail
  {
    template< class T >
    class Node
    {
    public:
      explicit Node(FakeTag) noexcept;

      template< class... Args >
      explicit Node(Args&&... args)
        noexcept(std::is_nothrow_constructible< T, Args... >::value);

      void destroyValue() noexcept;

      T& value() noexcept;
      const T& value() const noexcept;

      Node< T >* getNext() noexcept;
      const Node< T >* getNext() const noexcept;

    private:
      alignas(T) unsigned char storage_[sizeof(T)];
      Node< T >* next_;

      friend class List< T >;
      friend class LIter< T >;
      friend class LCIter< T >;

      template< class Key, class Value, class Hash, class Equal >
      friend class HashTable;

      template< class Key, class Value, class Hash, class Equal >
      friend class HashIter;

      template< class Key, class Value, class Hash, class Equal >
      friend class HashConstIter;
    };
  }

  template< class T >
  class List
  {
  public:
    List();
    List(const List& other);
    List(List&& other) noexcept;
    explicit List(const T& value);
    ~List() noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > insert(LIter< T > it, const T& value);
    LIter< T > erase(LIter< T > it) noexcept;
    void pushBack(const T& value);
    void swap(List& other) noexcept;
    void clear() noexcept;

    void splice_after(LIter< T > pos, List& other) noexcept;
    void splice_after(LIter< T > pos, List& other, LIter< T > it) noexcept;
    void splice_after(LIter< T > pos, List& other, LIter< T > first, LIter< T > last) noexcept;

    void merge(List& other) noexcept;
    template< class Compare >
    void merge(List& other, Compare comp) noexcept;

    void sort();
    template< class Compare >
    void sort(Compare comp);

    template< class P >
    LIter< T > partition(P p);

    template< class... Args >
    LIter< T > emplace_front(Args&&... args);

    template< class... Args >
    LIter< T > emplace_after(LIter< T > pos, Args&&... args);

    template< class... Args >
    LIter< T > emplace_back(Args&&... args);

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;

    size_t getsize() const noexcept;
    detail::Node< T >* getfirst() const noexcept;

    template< class P >
    bool erase_if(P p);

  private:
    detail::Node< T >* fake_node_;
    size_t size_;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashTable;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashIter;

    template< class Key, class Value, class Hash, class Equal >
    friend class HashConstIter;
  };

  template< class T >
  LIter< T >::LIter() noexcept:
    ptr_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T >* p) noexcept:
    ptr_(p)
  {}

  template< class T >
  T& LIter< T >::operator*() noexcept
  {
    return ptr_->value();
  }

  template< class T >
  T* LIter< T >::operator->() noexcept
  {
    return std::addressof(ptr_->value());
  }

  template< class T >
  const T& LIter< T >::operator*() const noexcept
  {
    return ptr_->value();
  }

  template< class T >
  const T* LIter< T >::operator->() const noexcept
  {
    return std::addressof(ptr_->value());
  }

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T >* p) noexcept:
    ptr_(p)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& it) noexcept:
    ptr_(it.ptr_)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return ptr_->value();
  }

  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    return std::addressof(ptr_->value());
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  detail::Node< T >::Node(FakeTag) noexcept:
    next_(nullptr)
  {}

  template< class T >
  template< class... Args >
  detail::Node< T >::Node(Args&&... args)
    noexcept(std::is_nothrow_constructible< T, Args... >::value):
    next_(nullptr)
  {
    new (storage_) T(std::forward< Args >(args)...);
  }

  template< class T >
  void detail::Node< T >::destroyValue() noexcept
  {
    reinterpret_cast< T* >(storage_)->~T();
  }

  template< class T >
  T& detail::Node< T >::value() noexcept
  {
    return *reinterpret_cast< T* >(storage_);
  }

  template< class T >
  const T& detail::Node< T >::value() const noexcept
  {
    return *reinterpret_cast< const T* >(storage_);
  }

  template< class T >
  detail::Node< T >* detail::Node< T >::getNext() noexcept
  {
    return next_;
  }

  template< class T >
  const detail::Node< T >* detail::Node< T >::getNext() const noexcept
  {
    return next_;
  }

  template< class T >
  List< T >::List():
    size_(0)
  {
    void* mem = ::operator new(sizeof(detail::Node< T >));
    fake_node_ = new (mem) detail::Node< T >(detail::FakeTag{});
    fake_node_->next_ = fake_node_;
  }

  template< class T >
  List< T >::List(const List& other):
    size_(0)
  {
    void* mem = ::operator new(sizeof(detail::Node< T >));
    fake_node_ = new (mem) detail::Node< T >(detail::FakeTag{});
    fake_node_->next_ = fake_node_;

    try
    {
      for (auto it = other.cbegin(); it != other.cend(); ++it)
      {
        pushBack(*it);
      }
    }
    catch (...)
    {
      clear();

      fake_node_->~Node();
      ::operator delete(mem);
      fake_node_ = nullptr;
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    fake_node_(std::exchange(other.fake_node_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class T >
  List< T >::List(const T& value):
    size_(0)
  {
    void* mem = ::operator new(sizeof(detail::Node< T >));
    fake_node_ = new (mem) detail::Node< T >(detail::FakeTag{});
    fake_node_->next_ = fake_node_;

    try
    {
      pushBack(value);
    }
    catch (...)
    {
      fake_node_->~Node();
      ::operator delete(mem);
      fake_node_ = nullptr;
      throw;
    }
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();

    if (fake_node_)
    {
      fake_node_->~Node();
      ::operator delete(fake_node_);
      fake_node_ = nullptr;
    }
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != std::addressof(other))
    {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      List tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (size_ > 0)
    {
      erase(end());
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > it, const T& value)
  {
    void* mem = ::operator new(sizeof(detail::Node< T >));
    detail::Node< T >* new_node = new (mem) detail::Node< T >(value);

    if (fake_node_->next_ == fake_node_)
    {
      new_node->next_ = fake_node_;
      fake_node_->next_ = new_node;
    }
    else
    {
      new_node->next_ = it.ptr_->next_;
      it.ptr_->next_ = new_node;
    }
    ++size_;

    return LIter< T >(new_node);
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > it) noexcept
  {
    if (!fake_node_ || it.ptr_->next_ == fake_node_)
    {
      return end();
    }
    detail::Node< T >* to_delete = it.ptr_->next_;
    it.ptr_->next_ = to_delete->next_;

    to_delete->destroyValue();
    to_delete->~Node();
    ::operator delete (to_delete);
    --size_;
    return it;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    detail::Node< T >* last = fake_node_;
    while (last->next_ != fake_node_)
    {
      last = last->next_;
    }
    insert(LIter< T >(last), value);
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List& other) noexcept
  {
    if (other.size_ == 0)
    {
      return;
    }

    splice_after(pos, other, other.end(), other.end());
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List& other, LIter< T > it) noexcept
  {
    if (it.ptr_->next_ == other.fake_node_)
    {
      return;
    }

    LIter< T > last_range(it.ptr_->next_->next_);
    splice_after(pos, other, it, last_range);
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List& other, LIter< T > first, LIter< T > last) noexcept
  {
    if (std::addressof(other) == this)
    {
      return;
    }

    detail::Node< T >* range_start = first.ptr_->next_;
    detail::Node< T >* range_end = last.ptr_;
    if (range_start == range_end)
    {
      return;
    }

    detail::Node< T >* range_tail = range_start;
    size_t count = 1;
    while (range_tail->next_ != range_end)
    {
      range_tail = range_tail->next_;
      ++count;
    }

    first.ptr_->next_ = range_end;
    detail::Node< T >* saved_next = pos.ptr_->next_;
    pos.ptr_->next_ = range_start;
    range_tail->next_ = saved_next;

    size_ += count;
    other.size_ -= count;
  }

  template< class T >
  void List< T >::merge(List& other) noexcept
  {
    merge(other, std::less< T >{});
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List& other, Compare comp) noexcept
  {
    if (other.size_ == 0)
    {
      return;
    }

    LIter< T > curr = end();
    LIter< T > prev = other.end();

    while (prev.ptr_->next_ != other.fake_node_)
    {
      while (curr.ptr_->next_ != fake_node_ &&
        comp(curr.ptr_->next_->value(), prev.ptr_->next_->value()))
      {
        ++curr;
      }

      if (curr.ptr_->next_ == fake_node_)
      {
        splice_after(curr, other);
        break;
      }

      splice_after(curr, other, prev);
      ++curr;
    }
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >{});
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    if (size_ <= 1)
    {
      return;
    }

    List< T > second_half;
    LIter< T > mid = begin();
    size_t half_size = size_ / 2 - 1;

    for (size_t i = 0; i < half_size; ++i)
    {
      ++mid;
    }

    second_half.splice_after(second_half.end(), *this, mid, end());

    if (second_half.size_ == 0)
    {
      return;
    }

    sort(comp);
    second_half.sort(comp);
    merge(second_half, comp);
  }

  template< class T >
  template< class P >
  LIter< T > List< T >::partition(P p)
  {
    List< T > false_list;
    LIter< T > false_tail = false_list.end();
    LIter< T > curr = end();

    while (curr.ptr_->next_ != fake_node_)
    {
      if (!p(curr.ptr_->next_->value()))
      {
        false_list.splice_after(false_tail, *this, curr);
        ++false_tail;
      }
      else
      {
        ++curr;
      }
    }

    splice_after(curr, false_list);
    return (curr.ptr_->next_ == fake_node_) ? end() : LIter< T >(curr.ptr_->next_);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplace_front(Args&&... args)
  {
    return emplace_after(end(), std::forward< Args >(args)...);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplace_after(LIter< T > pos, Args&&... args)
  {
    void* mem = ::operator new(sizeof(detail::Node< T >));
    detail::Node< T >* new_node = new (mem) detail::Node< T >(std::forward< Args >(args)...);

    if (fake_node_->next_ == fake_node_)
    {
      new_node->next_ = fake_node_;
      fake_node_->next_ = new_node;
    }
    else
    {
      new_node->next_ = pos.ptr_->next_;
      pos.ptr_->next_ = new_node;
    }
    ++size_;

    return LIter< T >(new_node);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplace_back(Args&&... args)
  {
    detail::Node< T >* last = fake_node_;

    while (last->next_ != fake_node_)
    {
      last = last->next_;
    }

    return emplace_after(LIter< T >(last), std::forward< Args >(args)...);
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake_node_->next_);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(fake_node_);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(fake_node_->next_);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(fake_node_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(fake_node_->next_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(fake_node_);
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(fake_node_, other.fake_node_);
    std::swap(size_, other.size_);
  }

  template< class T >
  size_t List< T >::getsize() const noexcept
  {
    return size_;
  }

  template< class T >
  detail::Node< T >* List< T >::getfirst() const noexcept
  {
    return fake_node_->next_;
  }

  template< class T >
  template< class P >
  bool List< T >::erase_if(P p)
  {
    bool was_erased = false;
    detail::Node< T >* prev = fake_node_;
    detail::Node< T >* curr = prev->next_;

    while (curr != fake_node_)
    {
      if (p(curr->value()))
      {
        erase(LIter< T >(prev));
        curr = prev->next_;
        was_erased = true;
      }
      else
      {
        prev = curr;
        curr = curr->next_;
      }
    }
    return was_erased;
  }
}

#endif

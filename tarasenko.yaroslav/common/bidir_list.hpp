#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP

#include <utility>
#include <cstddef>
#include <memory>

namespace tarasenko
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }

  template< class T >
  class BidirList;

  template< class T >
  class ListIter
  {
  public:
    ListIter() noexcept;
    bool operator==(const ListIter< T >& it) const noexcept;
    bool operator!=(const ListIter< T >& it) const noexcept;
    T& operator*();
    T* operator->();
    ListIter< T >& operator++() noexcept;
    ListIter< T > operator++(int) noexcept;
    ListIter< T >& operator--() noexcept;
    ListIter< T > operator--(int) noexcept;
  private:
    friend class BidirList< T >;
    detail::Node< T >* ptr_;
    const BidirList< T >* owner_;
    ListIter(detail::Node< T >* node, const BidirList< T >* list) noexcept;
  };

  template< class T >
  class ListConstIter
  {
  public:
    ListConstIter() noexcept;
    bool operator==(const ListConstIter< T >& it) const noexcept;
    bool operator!=(const ListConstIter< T >& it) const noexcept;
    const T& operator*() const;
    const T* operator->() const;
    ListConstIter< T >& operator++() noexcept;
    ListConstIter< T > operator++(int) noexcept;
    ListConstIter< T >& operator--() noexcept;
    ListConstIter< T > operator--(int) noexcept;
  private:
    friend class BidirList< T >;
    detail::Node< T >* ptr_;
    const BidirList< T >* owner_;
    ListConstIter(detail::Node< T >* node, const BidirList< T >* list) noexcept;
  };

  template< class T >
  class BidirList
  {
  public:
    BidirList();
    BidirList(const BidirList< T >& list);
    BidirList(BidirList< T >&& list) noexcept;
    ~BidirList() noexcept;
    BidirList< T >& operator=(const BidirList< T >& list);
    BidirList< T >& operator=(BidirList< T >&& list) noexcept;
    size_t size() const noexcept;
    ListIter< T > begin() noexcept;
    ListConstIter< T > begin() const noexcept;
    ListConstIter< T > cbegin() const noexcept;
    ListIter< T > end() noexcept;
    ListConstIter< T > end() const noexcept;
    ListConstIter< T > cend() const noexcept;
    void push_back(const T& val);
    void push_front(const T& val);
    void push_back(T&& val);
    void push_front(T&& val);
    bool empty() const noexcept;
    const T& front() const;
    const T& back() const;
    T& front();
    T& back();
    ListIter< T > erase(ListIter< T > it) noexcept;
    ListIter< T > erase(ListIter< T > start, ListIter< T > end) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;
    ListIter< T > insert(ListIter< T > it, const T& val);
    ListIter< T > insert(ListIter< T > it, T&& val);
    void clear() noexcept;
    void swap(BidirList< T >& list1, BidirList< T >& list2) noexcept;
  private:
    friend class ListIter< T >;
    friend class ListConstIter< T >;
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    size_t size_;
    ListIter< T > insert_node(ListIter< T > it, detail::Node< T >* new_node) noexcept;
  };

  template< class T >
  BidirList< T >::BidirList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  BidirList< T >::~BidirList() noexcept
  {
    clear();
  }

  template< class T >
  size_t BidirList< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  ListIter< T > BidirList< T >::begin() noexcept
  {
    return ListIter< T >(head_, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::begin() const noexcept
  {
    return ListConstIter< T >(head_, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cbegin() const noexcept
  {
    return ListConstIter< T >(head_, this);
  }

  template< class T >
  ListIter< T > BidirList< T >::end() noexcept
  {
    return ListIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::end() const noexcept
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListConstIter< T > BidirList< T >::cend() const noexcept
  {
    return ListConstIter< T >(nullptr, this);
  }

  template< class T >
  ListIter< T >::ListIter() noexcept:
    ptr_(nullptr),
    owner_(nullptr)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter() noexcept:
    ptr_(nullptr),
    owner_(nullptr)
  {}

  template< class T >
  ListIter< T >::ListIter(detail::Node< T >* node, const BidirList< T >* list) noexcept:
    ptr_(node),
    owner_(list)
  {}

  template< class T >
  ListConstIter< T >::ListConstIter(detail::Node< T >* node, const BidirList< T >* list) noexcept:
    ptr_(node),
    owner_(list)
  {}

  template< class T >
  bool ListIter< T >::operator==(const ListIter< T >& it) const noexcept
  {
    return ptr_ == it.ptr_;
  }

  template< class T >
  bool ListIter< T >::operator!=(const ListIter< T >& it) const noexcept
  {
    return !(ptr_ == it.ptr_);
  }

  template< class T >
  bool ListConstIter< T >::operator==(const ListConstIter< T >& it) const noexcept
  {
    return ptr_ == it.ptr_;
  }

  template< class T >
  bool ListConstIter< T >::operator!=(const ListConstIter< T >& it) const noexcept
  {
    return !(ptr_ == it.ptr_);
  }

  template< class T >
  void BidirList< T >::push_back(const T& val)
  {
    insert(end(), val);
  }

  template< class T >
  void BidirList< T >::push_back(T&& val)
  {
    insert(end(), std::forward< T >(val));
  }

  template< class T >
  void BidirList< T >::push_front(const T& val)
  {
    insert(begin(), val);
  }

  template< class T >
  void BidirList< T >::push_front(T&& val)
  {
    insert(begin(), std::forward< T >(val));
  }

  template< class T >
  T& ListIter< T >::operator*()
  {
    return ptr_->val;
  }

  template< class T >
  const T& ListConstIter< T >::operator*() const
  {
    return ptr_->val;
  }

  template< class T >
  T* ListIter< T >::operator->()
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  const T* ListConstIter< T >::operator->() const
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator++(int) noexcept
  {
    ListIter< T > copy(*this);
    ptr_ = ptr_->next;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator++(int) noexcept
  {
    ListConstIter< T > copy(*this);
    ptr_ = ptr_->next;
    return copy;
  }

  template< class T >
  ListIter< T >& ListIter< T >::operator--() noexcept
  {
    if (ptr_ == nullptr)
    {
      ptr_ = owner_->tail_;
      return *this;
    }
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  ListConstIter< T >& ListConstIter< T >::operator--() noexcept
  {
    if (ptr_ == nullptr)
    {
      ptr_ = owner_->tail_;
      return *this;
    }
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  ListIter< T > ListIter< T >::operator--(int) noexcept
  {
    if (ptr_ == nullptr)
    {
      ptr_ = owner_->tail_;
      return *this;
    }
    ListIter< T > copy(*this);
    ptr_ = ptr_->prev;
    return copy;
  }

  template< class T >
  ListConstIter< T > ListConstIter< T >::operator--(int) noexcept
  {
    if (ptr_ == nullptr)
    {
      ptr_ = owner_->tail_;
      return *this;
    }
    ListConstIter< T > copy(*this);
    ptr_ = ptr_->prev;
    return copy;
  }

  template< class T >
  bool BidirList< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  const T& BidirList< T >::front() const
  {
    return head_->val;
  }

  template< class T >
  const T& BidirList< T >::back() const
  {
    return tail_->val;
  }

  template< class T >
  T& BidirList< T >::front()
  {
    return head_->val;
  }

  template< class T >
  T& BidirList< T >::back()
  {
    return tail_->val;
  }

  template< class T >
  ListIter< T > BidirList< T >::erase(ListIter< T > it) noexcept
  {
    detail::Node< T >* next = it.ptr_->next;
    detail::Node< T >* prev = it.ptr_->prev;
    delete it.ptr_;
    if (next != nullptr)
    {
      next->prev = prev;
    }
    else
    {
      tail_ = prev;
    }

    if (prev != nullptr)
    {
      prev->next = next;
    }
    else
    {
      head_ = next;
    }
    it.ptr_ = next;
    size_--;
    return it;
  }

  template< class T >
  ListIter< T > BidirList< T >::erase(ListIter< T > first, ListIter< T > last) noexcept
  {
    while (first != last)
    {
      first = erase(first);
    }
    return first;
  }

  template< class T >
  void BidirList< T >::pop_front() noexcept
  {
    erase(begin());
  }

  template< class T >
  void BidirList< T >::pop_back() noexcept
  {
    erase(--end());
  }

  template< class T >
  ListIter< T > BidirList< T >::insert(ListIter< T > it, const T& val)
  {
    return insert_node(it, new detail::Node< T >{val, nullptr, nullptr});
  }

  template< class T >
  ListIter< T > BidirList< T >::insert(ListIter< T > it, T&& val)
  {
    return insert_node(it, new detail::Node< T >{std::forward< T >(val), nullptr, nullptr});
  }

  template< class T >
  ListIter< T > BidirList< T >::insert_node(ListIter< T > it, detail::Node< T >* new_node) noexcept
  {
    if (empty())
    {
      head_ = new_node;
      tail_ = new_node;
    }
    else if (it.ptr_ == nullptr)
    {
      new_node->prev = tail_;
      tail_->next = new_node;
      tail_ = new_node;
    }
    else
    {
      new_node->next = it.ptr_;
      new_node->prev = it.ptr_->prev;
      if (it.ptr_->prev == nullptr)
      {
        head_->prev = new_node;
        head_ = new_node;
      }
      else
      {
        it.ptr_->prev->next = new_node;
        it.ptr_->prev = new_node;
      }
    }
    size_++;
    return ListIter< T >(new_node, this);
  }

  template< class T >
  void BidirList< T >::clear() noexcept
  {
    erase(begin(), end());
  }

  template< class T >
  BidirList< T >::BidirList(const BidirList< T >& list):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (ListConstIter< T > it = list.begin(); it != list.end(); ++it)
      {
        push_back(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  BidirList< T >::BidirList(BidirList< T >&& list) noexcept:
    head_(std::exchange(list.head_, nullptr)),
    tail_(std::exchange(list.tail_, nullptr)),
    size_(std::exchange(list.size_, 0))
  {}

  template< class T >
  void BidirList< T >::swap(BidirList< T >& list1, BidirList< T >& list2) noexcept
  {
    std::swap(list1.head_, list2.head_);
    std::swap(list1.tail_, list2.tail_);
    std::swap(list1.size_, list2.size_);
  }

  template< class T >
  BidirList< T >& BidirList< T >::operator=(const BidirList< T >& list)
  {
    BidirList< T > temp(list);
    swap(*this, temp);
    return *this;
  }

  template< class T >
  BidirList< T >& BidirList< T >::operator=(BidirList< T >&& list) noexcept
  {
    if (this == std::addressof(list))
    {
      return *this;
    }
    BidirList< T > temp(std::move(list));
    swap(*this, temp);
    return *this;
  }
}

#endif

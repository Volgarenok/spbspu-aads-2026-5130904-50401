#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include <cstddef>
#include <limits>

namespace sedov
{
  const size_t MAX = std::numeric_limits< size_t >::max();

  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T val_;
    Node< T > * next_;
    Node< T > * prev_;
    Node(const T & value):
      val_(value), next_(nullptr), prev_(nullptr)
    {}
    Node(T && value):
      val_(value), next_(nullptr), prev_(nullptr)
    {}
  };

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T > * ptr_;
  public:
    LIter(Node< T > * p = nullptr):
      ptr_(p)
    {}

    T & operator*() const
    {
      return ptr_->val_;
    }

    T * operator->() const
    {
      return &ptr_->val_;
    }

    LIter & operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LIter operator++()
    {
      LIter temp = *this;
      ptr_ = ptr_->next_;
      return temp;
    }

    LIter & operator--()
    {
      ptr_ = ptr_->prev_;
      return *this;
    }

    LIter operator--()
    {
      LIter temp = *this;
      ptr_ = ptr_->prev_;
      return temp;
    }

    bool operator==(const LIter & other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter & other) const
    {
      return !(ptr_ == other.ptr_);
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T > * ptr_;
  public:
    LCIter(const Node< T > * p = nullptr):
      ptr_(p)
    {}

    const T & operator*() const
    {
        return ptr_->val_;
    }

    const T * operator->() const
    {
        return &ptr_->val_;
    }

    LCIter & operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    LCIter operator++(int)
    {
        LCIter tmp = *this;
        ptr_ = ptr_->next_;
        return tmp;
    }

    LCIter & operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    LCIter operator--(int)
    {
        LCIter tmp = *this;
        ptr_ = ptr_->prev_;
        return tmp;
    }

    bool operator==(const LCIter & other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter & other) const
    {
        return !(ptr_ == other.ptr_);
    }
  };

  template< class T >
  class List
  {
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  public:
    List():
      head_(nullptr), tail_(nullptr), size_(0)
    {}

    List(const List & h):
      List()
    {
      for (Node< T > * cur = h.head_; cur != nullptr; cur = cur->next_)
      {
        push_back(cur->val_);
      }
    }

    List(List && h) noexcept:
      head_(h.head_), tail_(h.tail_), size_(h.size_)
    {
      h.head_ = nullptr;
      h.tail_ = nullptr;
      h.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List & operator=(const List & h)
    {
      if (this != &h)
      {
        List temp(h);
        swap(temp);
      }
      return *this;
    }

    LIter< T > begin()
    {
      return LIter< T >(head_);
    }

    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(head_);
    }

    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    T & front()
    {
      return head_->val_;
    }

    const T & front() const
    {
      return head_->val_;
    }

    T & back()
    {
      return tail_->val_;
    }

    const T & back() const
    {
      return tail_->val_;
    }
  };
}

#endif
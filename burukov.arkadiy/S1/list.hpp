#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace burukov
{
  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T val_;
    Node< T > *next_;
    Node(const T &value, Node< T > *next):
      val_(value),
      next_(next)
    {}
  };

  template< class T >
  class LIter
  {
    friend class List< T >;
  public:
    LIter():
      ptr_(nullptr)
    {}
    explicit LIter(Node< T > *p):
      ptr_(p)
    {}
    T &operator*()
    {
      return ptr_->val_;
    }
    T *operator->()
    {
      return &(ptr_->val_);
    }
    LIter< T > &operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }
    LIter< T > operator++(int)
    {
      LIter< T > temp(*this);
      ptr_ = ptr_->next_;
      return temp;
    }
    bool operator==(const LIter< T > &rhs) const
    {
      return ptr_ == rhs.ptr_;
    }
    bool operator!=(const LIter< T > &rhs) const
    {
      return ptr_ != rhs.ptr_;
    }

  private:
    Node< T > *ptr_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter():
      ptr_(nullptr)
    {}
    explicit LCIter(const Node< T > *p):
      ptr_(p)
    {}
    const T &operator*() const
    {
      return ptr_->val_;
    }
    const T *operator->() const
    {
      return &(ptr_->val_);
    }
    LCIter< T > &operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }
    LCIter< T > operator++(int)
    {
      LCIter< T > temp(*this);
      ptr_ = ptr_->next_;
      return temp;
    }
    bool operator==(const LCIter< T > &rhs) const
    {
      return ptr_ == rhs.ptr_;
    }
    bool operator!=(const LCIter< T > &rhs) const
    {
      return ptr_ != rhs.ptr_;
    }

  private:
    const Node< T > *ptr_;
  };
}

#endif
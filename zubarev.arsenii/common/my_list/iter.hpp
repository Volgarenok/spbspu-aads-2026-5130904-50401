#ifndef ITER_HPP
#define ITER_HPP

#include <memory>

namespace zubarev
{
  template< class T >
  class List;

  namespace detail
  {
    template< class T >
    struct Node;
  }

  template< class T >
  class LIter
  {
  public:
    T& operator*();
    T* operator->();
    LIter< T >& operator++();
    LIter< T > operator++(int);

    bool operator!=(const LIter&) const;
    bool operator==(const LIter&) const;

  private:
    friend class List< T >;
    detail::Node< T >* ptr_;
    LIter(detail::Node< T >* p);
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T >* p):
    ptr_(p)
  {}

  template< class T >
  T& LIter< T >::operator*()
  {
    return ptr_->val;
  }

  template< class T >
  T* LIter< T >::operator->()
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > tmp = *this;
    ptr_ = ptr_->next;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& iter) const
  {
    return ptr_ != iter.ptr_;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& iter) const
  {
    return ptr_ == iter.ptr_;
  }
}

#endif

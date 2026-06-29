#ifndef CITER_HPP
#define CITER_HPP
#include <memory>
namespace zubarev
{
  template< class T >
  class List;
  template< class T >
  class LIter;
  namespace detail
  {
    template< class T >
    struct Node;
  }

  template< class T >
  class LCIter
  {

  public:
    LCIter(const LIter< T >& other);
    const T& operator*() const;
    const T* operator->() const;
    LCIter< T >& operator++();
    LCIter< T > operator++(int);
    bool operator!=(const LCIter&) const;
    bool operator==(const LCIter&) const;

  private:
    friend class List< T >;
    const detail::Node< T >* ptr_;
    LCIter(const detail::Node< T >* p);
  };

  template< typename T >
  LCIter< T >::LCIter(const LIter< T >& other):
    ptr_(other.ptr_)
  {}
  template< class T >
  LCIter< T >::LCIter(const detail::Node< T >* p):
    ptr_(p)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    return ptr_->val;
  }
  template< class T >
  const T* LCIter< T >::operator->() const
  {
    return std::addressof(ptr_->val);
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    ptr_ = ptr_->next;
    return *this;
  }
  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > tmp = this;
    ptr_ = ptr_->next;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& iter) const
  {
    return ptr_ != iter.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& iter) const
  {
    return ptr_ == iter.ptr_;
  }
}

#endif

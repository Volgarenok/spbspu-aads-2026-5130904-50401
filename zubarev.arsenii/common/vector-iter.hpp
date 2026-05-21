#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cstddef>
namespace topit
{
  template < class T >
  struct Vector;
  template < class T >
  class VectIter
  {
    friend struct topit::Vector< T >;
    VectIter() noexcept:
      ptr_(nullptr)
    {}
    VectIter(T* ptr) noexcept:
      ptr_(ptr)
    {}

  public:
    T& operator*();
    VectIter< T >& operator++();
    VectIter< T > operator++(int);
    VectIter< T > operator+(int);

    VectIter< T >& operator--();
    VectIter< T > operator--(int);
    size_t operator-(const VectIter< T >&) const;
    VectIter< T > operator-(const size_t& other) const;
    bool operator!=(const VectIter< T >&) const;
    bool operator==(const VectIter< T >&) const;

  private:
    T* ptr_;
  };

  template < class T >
  T& topit::VectIter< T >::operator*()
  {
    return *ptr_;
  }

  template < class T >
  VectIter< T >& topit::VectIter< T >::operator++()
  {
    ++ptr_;
    return *this;
  }

  template < class T >
  VectIter< T > topit::VectIter< T >::operator++(int)
  {
    VectIter< T > tmp = *this;
    ++ptr_;
    return tmp;
  }
  template < class T >
  VectIter< T > topit::VectIter< T >::operator+(int n)
  {
    return VectIter(ptr_ + n);
  }
  template < class T >
  VectIter< T >& topit::VectIter< T >::operator--()
  {
    --ptr_;
    return *this;
  }

  template < class T >
  VectIter< T > topit::VectIter< T >::operator--(int)
  {
    VectIter< T > tmp = *this;
    --ptr_;
    return tmp;
  }
  template < class T >
  size_t topit::VectIter< T >::operator-(const VectIter< T >& other) const
  {
    return static_cast< size_t >(ptr_ - other.ptr_);
  }
    template < class T >
  VectIter< T > topit::VectIter< T >::operator-(const size_t& other) const
  {

    return (ptr_ - other);
  }
  template < class T >
  bool topit::VectIter< T >::operator!=(const VectIter< T >& other) const
  {
    return ptr_ != other.ptr_;
  }
  template < class T >
  bool topit::VectIter< T >::operator==(const VectIter< T >& other) const
  {
    return ptr_ == other.ptr_;
  }
}

#endif

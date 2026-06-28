#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>
#include <iterator>

namespace zubarev
{
  template< class T >
  struct Vector;

  template< class T >
  class VectIter
  {
    friend struct zubarev::Vector< T >;

    VectIter() noexcept:
      ptr_(nullptr)
    {}

    VectIter(T* ptr) noexcept:
      ptr_(ptr)
    {}

  public:
    using iterator = VectIter<T>;
    using const_iterator = VectIter<const T>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    T& operator*() const;
    T* operator->() const;

    VectIter& operator++();
    VectIter operator++(int);

    VectIter& operator--();
    VectIter operator--(int);

    VectIter operator+(difference_type n) const;
    VectIter operator-(difference_type n) const;

    VectIter& operator+=(difference_type n);
    VectIter& operator-=(difference_type n);

    difference_type operator-(const VectIter& other) const;

    bool operator!=(const VectIter& other) const;
    bool operator==(const VectIter& other) const;

    bool operator<(const VectIter& other) const;
    bool operator>(const VectIter& other) const;
    bool operator<=(const VectIter& other) const;
    bool operator>=(const VectIter& other) const;

    T& operator[](difference_type index) const;

  private:
    T* ptr_;
  };

  template< class T >
  T& VectIter< T >::operator*() const
  {
    return *ptr_;
  }

  template< class T >
  T* VectIter< T >::operator->() const
  {
    return ptr_;
  }

  template< class T >
  VectIter< T >& VectIter< T >::operator++()
  {
    ++ptr_;
    return *this;
  }

  template< class T >
  VectIter< T > VectIter< T >::operator++(int)
  {
    VectIter tmp(*this);
    ++ptr_;
    return tmp;
  }

  template< class T >
  VectIter< T >& VectIter< T >::operator--()
  {
    --ptr_;
    return *this;
  }

  template< class T >
  VectIter< T > VectIter< T >::operator--(int)
  {
    VectIter tmp(*this);
    --ptr_;
    return tmp;
  }

  template< class T >
  VectIter< T > VectIter< T >::operator+(difference_type n) const
  {
    return VectIter(ptr_ + n);
  }

  template< class T >
  VectIter< T > VectIter< T >::operator-(difference_type n) const
  {
    return VectIter(ptr_ - n);
  }

  template< class T >
  VectIter< T >& VectIter< T >::operator+=(difference_type n)
  {
    ptr_ += n;
    return *this;
  }

  template< class T >
  VectIter< T >& VectIter< T >::operator-=(difference_type n)
  {
    ptr_ -= n;
    return *this;
  }

  template< class T >
  typename VectIter< T >::difference_type VectIter< T >::operator-(const VectIter& other) const
  {
    return ptr_ - other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator!=(const VectIter& other) const
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator==(const VectIter& other) const
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator<(const VectIter& other) const
  {
    return ptr_ < other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator>(const VectIter& other) const
  {
    return ptr_ > other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator<=(const VectIter& other) const
  {
    return ptr_ <= other.ptr_;
  }

  template< class T >
  bool VectIter< T >::operator>=(const VectIter& other) const
  {
    return ptr_ >= other.ptr_;
  }

  template< class T >
  T& VectIter< T >::operator[](difference_type index) const
  {
    return *(ptr_ + index);
  }

}

#endif

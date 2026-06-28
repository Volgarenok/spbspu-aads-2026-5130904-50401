#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <cstddef>
#include <iterator>

namespace zubarev
{
  template< class T >
  struct Vector;

  template< class T >
  class CVectIter
  {
    friend struct zubarev::Vector< T >;

    CVectIter() noexcept:
      ptr_(nullptr)
    {}

    CVectIter(const T* ptr) noexcept:
      ptr_(ptr)
    {}

  public:
    using iterator = CVectIter<T>;
    using const_iterator = CVectIter<const T>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    const T& operator*() const;
    const T* operator->() const;

    CVectIter& operator++();
    CVectIter operator++(int);

    CVectIter& operator--();
    CVectIter operator--(int);

    CVectIter operator+(difference_type n) const;
    CVectIter operator-(difference_type n) const;

    CVectIter& operator+=(difference_type n);
    CVectIter& operator-=(difference_type n);

    difference_type operator-(const CVectIter& other) const;

    bool operator!=(const CVectIter& other) const;
    bool operator==(const CVectIter& other) const;

    bool operator<(const CVectIter& other) const;
    bool operator>(const CVectIter& other) const;
    bool operator<=(const CVectIter& other) const;
    bool operator>=(const CVectIter& other) const;

    const T& operator[](difference_type index) const;

  private:
    const T* ptr_;
  };

  template< class T >
  const T& CVectIter< T >::operator*() const
  {
    return *ptr_;
  }

  template< class T >
  const T* CVectIter< T >::operator->() const
  {
    return ptr_;
  }

  template< class T >
  CVectIter< T >& CVectIter< T >::operator++()
  {
    ++ptr_;
    return *this;
  }

  template< class T >
  CVectIter< T > CVectIter< T >::operator++(int)
  {
    CVectIter tmp(*this);
    ++ptr_;
    return tmp;
  }

  template< class T >
  CVectIter< T >& CVectIter< T >::operator--()
  {
    --ptr_;
    return *this;
  }

  template< class T >
  CVectIter< T > CVectIter< T >::operator--(int)
  {
    CVectIter tmp(*this);
    --ptr_;
    return tmp;
  }

  template< class T >
  CVectIter< T > CVectIter< T >::operator+(difference_type n) const
  {
    return CVectIter(ptr_ + n);
  }

  template< class T >
  CVectIter< T > CVectIter< T >::operator-(difference_type n) const
  {
    return CVectIter(ptr_ - n);
  }

  template< class T >
  CVectIter< T >& CVectIter< T >::operator+=(difference_type n)
  {
    ptr_ += n;
    return *this;
  }

  template< class T >
  CVectIter< T >& CVectIter< T >::operator-=(difference_type n)
  {
    ptr_ -= n;
    return *this;
  }

  template< class T >
  typename CVectIter< T >::difference_type CVectIter< T >::operator-(const CVectIter& other) const
  {
    return ptr_ - other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator==(const CVectIter& other) const
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator!=(const CVectIter& other) const
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator<(const CVectIter& other) const
  {
    return ptr_ < other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator>(const CVectIter& other) const
  {
    return ptr_ > other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator<=(const CVectIter& other) const
  {
    return ptr_ <= other.ptr_;
  }

  template< class T >
  bool CVectIter< T >::operator>=(const CVectIter& other) const
  {
    return ptr_ >= other.ptr_;
  }

  template< class T >
  const T& CVectIter< T >::operator[](difference_type index) const
  {
    return *(ptr_ + index);
  }

}

#endif

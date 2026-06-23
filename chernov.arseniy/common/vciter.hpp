#ifndef VCITER_HPP
#define VCITER_HPP

#include <cstddef>

namespace chernov {
  template< class T >
  struct Vector;

  template< class T >
  struct VCIter {
    VCIter();
    explicit VCIter(T * ptr);
    const T & operator*() const;
    const T * operator->() const;
    const T & operator[](std::ptrdiff_t n) const;
    VCIter< T > & operator++();
    VCIter< T > operator++(int);
    VCIter< T > & operator--();
    VCIter< T > operator--(int);
    VCIter< T > operator+(std::ptrdiff_t n) const;
    VCIter< T > operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const VCIter< T > & other) const;
    bool operator==(const VCIter< T > & other) const noexcept;
    bool operator!=(const VCIter< T > & other) const noexcept;
    bool operator<(const VCIter< T > & other) const noexcept;
    bool operator>(const VCIter< T > & other) const noexcept;
    bool operator<=(const VCIter< T > & other) const noexcept;
    bool operator>=(const VCIter< T > & other) const noexcept;
  private:
    friend struct Vector< T >;
    T * ptr_;
  };
}

template< class T >
chernov::VCIter< T >::VCIter():
  ptr_(nullptr)
{}

template< class T >
chernov::VCIter< T >::VCIter(T * ptr):
  ptr_(ptr)
{}

template< class T >
const T & chernov::VCIter< T >::operator*() const
{
  return *ptr_;
}

template< class T >
const T * chernov::VCIter< T >::operator->() const
{
  return ptr_;
}

template< class T >
const T & chernov::VCIter< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
chernov::VCIter< T > & chernov::VCIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
chernov::VCIter< T > chernov::VCIter< T >::operator++(int)
{
  VCIter old = *this;
  ++(*this);
  return old;
}

template< class T >
chernov::VCIter< T > & chernov::VCIter< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
chernov::VCIter< T > chernov::VCIter< T >::operator--(int)
{
  VCIter old = *this;
  --(*this);
  return old;
}

template< class T >
chernov::VCIter< T > chernov::VCIter< T >::operator+(std::ptrdiff_t n) const
{
  return VCIter< T >{ptr_ + n};
}

template< class T >
chernov::VCIter< T > chernov::VCIter< T >::operator-(std::ptrdiff_t n) const
{
  return VCIter< T >{ptr_ - n};
}

template< class T >
std::ptrdiff_t chernov::VCIter< T >::operator-(const VCIter< T > & other) const
{
  return ptr_ - other.ptr_;
}

template< class T >
bool chernov::VCIter< T >::operator==(const VCIter< T > & other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool chernov::VCIter< T >::operator!=(const VCIter< T > & other) const noexcept
{
  return !(*this == other);
}

template< class T >
bool chernov::VCIter< T >::operator<(const VCIter< T > & other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool chernov::VCIter< T >::operator>(const VCIter< T > & other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool chernov::VCIter< T >::operator<=(const VCIter< T > & other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool chernov::VCIter< T >::operator>=(const VCIter< T > & other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif

#ifndef VITER_HPP
#define VITER_HPP

#include <cstddef>

namespace chernov {
  template< class T >
  struct Vector;

  template< class T >
  struct VIter {
    VIter();
    explicit VIter(T * ptr);
    T & operator*() const;
    T * operator->() const;
    T & operator[](std::ptrdiff_t n) const;
    VIter< T > & operator++();
    VIter< T > operator++(int);
    VIter< T > & operator--();
    VIter< T > operator--(int);
    VIter< T > operator+(std::ptrdiff_t n) const;
    VIter< T > operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const VIter< T > & other) const;
    bool operator==(const VIter< T > & other) const noexcept;
    bool operator!=(const VIter< T > & other) const noexcept;
    bool operator<(const VIter< T > & other) const noexcept;
    bool operator>(const VIter< T > & other) const noexcept;
    bool operator<=(const VIter< T > & other) const noexcept;
    bool operator>=(const VIter< T > & other) const noexcept;
  private:
    friend struct Vector< T >;
    T * ptr_;
  };
}

template< class T >
chernov::VIter< T >::VIter():
  ptr_(nullptr)
{}

template< class T >
chernov::VIter< T >::VIter(T * ptr):
  ptr_(ptr)
{}

template< class T >
T & chernov::VIter< T >::operator*() const
{
  return *ptr_;
}

template< class T >
T * chernov::VIter< T >::operator->() const
{
  return ptr_;
}

template< class T >
T & chernov::VIter< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
chernov::VIter< T > & chernov::VIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
chernov::VIter< T > chernov::VIter< T >::operator++(int)
{
  VIter old = *this;
  ++(*this);
  return old;
}

template< class T >
chernov::VIter< T > & chernov::VIter< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
chernov::VIter< T > chernov::VIter< T >::operator--(int)
{
  VIter old = *this;
  --(*this);
  return old;
}

template< class T >
chernov::VIter< T > chernov::VIter< T >::operator+(std::ptrdiff_t n) const
{
  return VIter< T >{ptr_ + n};
}

template< class T >
chernov::VIter< T > chernov::VIter< T >::operator-(std::ptrdiff_t n) const
{
  return VIter< T >{ptr_ - n};
}

template< class T >
std::ptrdiff_t chernov::VIter< T >::operator-(const VIter< T > & other) const
{
  return ptr_ - other.ptr_;
}

template< class T >
bool chernov::VIter< T >::operator==(const VIter< T > & other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool chernov::VIter< T >::operator!=(const VIter< T > & other) const noexcept
{
  return !(*this == other);
}

template< class T >
bool chernov::VIter< T >::operator<(const VIter< T > & other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool chernov::VIter< T >::operator>(const VIter< T > & other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool chernov::VIter< T >::operator<=(const VIter< T > & other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool chernov::VIter< T >::operator>=(const VIter< T > & other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif

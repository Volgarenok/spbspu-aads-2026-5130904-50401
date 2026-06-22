#ifndef VITER_HPP
#define VITER_HPP
#include <cstddef>

namespace zharov
{
  template< class T >
  struct VIter
  {
    VIter();
    explicit VIter(T* ptr);
    T& operator*() const;
    T* operator->() const;
    T& operator[](std::ptrdiff_t n) const;
    VIter< T >& operator++();
    VIter< T > operator++(int);
    VIter< T >& operator--();
    VIter< T > operator--(int);
    VIter< T > operator-(std::ptrdiff_t n) const;
    VIter< T > operator+(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const VIter< T >& other);

    bool operator==(const VIter& other) const noexcept;
    bool operator!=(const VIter& other) const noexcept;
    bool operator>(const VIter& other) const noexcept;
    bool operator<(const VIter& other) const noexcept;
    bool operator>=(const VIter& other) const noexcept;
    bool operator<=(const VIter& other) const noexcept;

  private:
    T* ptr_;
  };
}

template< class T >
zharov::VIter< T >::VIter():
  ptr_(nullptr)
{}

template< class T >
zharov::VIter< T >::VIter(T* ptr):
  ptr_(ptr)
{}

template< class T >
T& zharov::VIter< T >::operator*() const
{
  return *ptr_;
}

template< class T >
T* zharov::VIter< T >::operator->() const
{
  return ptr_;
}

template< class T >
T& zharov::VIter< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
zharov::VIter< T >& zharov::VIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
zharov::VIter< T > zharov::VIter< T >::operator++(int)
{
  VIter< T > temp = *this;
  ++ptr_;
  return temp;
}

template< class T >
zharov::VIter< T >& zharov::VIter< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
zharov::VIter< T > zharov::VIter< T >::operator--(int)
{
  VIter< T > temp = *this;
  --ptr_;
  return temp;
}

template< class T >
zharov::VIter< T > zharov::VIter< T >::operator-(std::ptrdiff_t n) const
{
  return VIter< T >(ptr_ + n);
  ;
}

template< class T >
zharov::VIter< T > zharov::VIter< T >::operator+(std::ptrdiff_t n) const
{
  return VIter< T >(ptr_ + n);
}

template< class T >
std::ptrdiff_t zharov::VIter< T >::operator-(const VIter< T >& other)
{
  return ptr_ - other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator==(const VIter& other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator!=(const VIter& other) const noexcept
{
  return ptr_ != other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator>(const VIter& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator<(const VIter& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator>=(const VIter& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

template< class T >
bool zharov::VIter< T >::operator<=(const VIter& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

#endif

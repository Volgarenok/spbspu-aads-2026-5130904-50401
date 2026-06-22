#ifndef VCITER_HPP
#define VCITER_HPP
#include <cstddef>

namespace zharov
{
  template< class T >
  struct VCIter
  {
    VCIter();
    explicit VCIter(T* ptr);
    const T& operator*() const;
    const T* operator->() const;
    const T& operator[](std::ptrdiff_t n) const;

    VCIter< T >& operator++();
    VCIter< T > operator++(int);
    VCIter< T >& operator--();
    VCIter< T > operator--(int);
    VCIter< T > operator-(std::ptrdiff_t n) const;
    VCIter< T > operator+(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const VCIter< T >& other);

    bool operator==(const VCIter& other) const noexcept;
    bool operator!=(const VCIter& other) const noexcept;
    bool operator>(const VCIter& other) const noexcept;
    bool operator<(const VCIter& other) const noexcept;
    bool operator>=(const VCIter& other) const noexcept;
    bool operator<=(const VCIter& other) const noexcept;

  private:
    T* ptr_;
  };
}

template< class T >
zharov::VCIter< T >::VCIter():
  ptr_(nullptr)
{}

template< class T >
zharov::VCIter< T >::VCIter(T* ptr):
  ptr_(ptr)
{}

template< class T >
const T& zharov::VCIter< T >::operator*() const
{
  return *ptr_;
}

template< class T >
const T* zharov::VCIter< T >::operator->() const
{
  return ptr_;
}

template< class T >
const T& zharov::VCIter< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
zharov::VCIter< T >& zharov::VCIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
zharov::VCIter< T > zharov::VCIter< T >::operator++(int)
{
  VCIter< T > temp = *this;
  ++ptr_;
  return temp;
}

template< class T >
zharov::VCIter< T >& zharov::VCIter< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
zharov::VCIter< T > zharov::VCIter< T >::operator--(int)
{
  VCIter< T > temp = *this;
  --ptr_;
  return temp;
}

template< class T >
zharov::VCIter< T > zharov::VCIter< T >::operator-(std::ptrdiff_t n) const
{
  return VCIter< T >(ptr_ + n);
  ;
}

template< class T >
zharov::VCIter< T > zharov::VCIter< T >::operator+(std::ptrdiff_t n) const
{
  return VCIter< T >(ptr_ + n);
}

template< class T >
std::ptrdiff_t zharov::VCIter< T >::operator-(const VCIter< T >& other)
{
  return ptr_ - other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator==(const VCIter& other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator!=(const VCIter& other) const noexcept
{
  return ptr_ != other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator>(const VCIter& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator<(const VCIter& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator>=(const VCIter& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

template< class T >
bool zharov::VCIter< T >::operator<=(const VCIter& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

#endif

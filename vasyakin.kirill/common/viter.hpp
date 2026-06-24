#ifndef VITER_HPP
#define VITER_HPP
#include <cstddef>

namespace vasyakin
{
  template< class T >
  struct Vector;

  template< class T >
  class VectorIterator
  {
  public:
    explicit VectorIterator(T* ptr) noexcept;

    template< class U >
    VectorIterator(const VectorIterator< U >& other) noexcept;

    T& operator*() const noexcept;
    T* operator->() const noexcept;
    T& operator[](std::ptrdiff_t i) const noexcept;

    VectorIterator& operator++() noexcept;
    VectorIterator operator++(int) noexcept;
    VectorIterator& operator--() noexcept;
    VectorIterator operator--(int) noexcept;

    VectorIterator& operator+=(std::ptrdiff_t n) noexcept;
    VectorIterator& operator-=(std::ptrdiff_t n) noexcept;
    VectorIterator operator+(std::ptrdiff_t n) const noexcept;
    VectorIterator operator-(std::ptrdiff_t n) const noexcept;
    std::ptrdiff_t operator-(const VectorIterator& other) const noexcept;

    bool operator==(const VectorIterator& other) const noexcept;
    bool operator!=(const VectorIterator& other) const noexcept;
    bool operator<(const VectorIterator& other) const noexcept;
    bool operator>(const VectorIterator& other) const noexcept;
    bool operator<=(const VectorIterator& other) const noexcept;
    bool operator>=(const VectorIterator& other) const noexcept;

  private:
    T* ptr_;
    friend struct Vector< T >;
    template< class U >
    friend class VectorIterator;
  };
}

template< class T >
vasyakin::VectorIterator< T >::VectorIterator(T* ptr) noexcept:
  ptr_(ptr)
{}

template< class T >
template< class U >
vasyakin::VectorIterator< T >::VectorIterator(const VectorIterator< U >& other) noexcept:
  ptr_(other.ptr_)
{}

template< class T >
T& vasyakin::VectorIterator< T >::operator*() const noexcept
{
  return *ptr_;
}

template< class T >
T* vasyakin::VectorIterator< T >::operator->() const noexcept
{
  return ptr_;
}

template< class T >
T& vasyakin::VectorIterator< T >::operator[](std::ptrdiff_t i) const noexcept
{
  return (ptr_[i]);
}

template< class T >
vasyakin::VectorIterator< T >& vasyakin::VectorIterator< T >::operator++() noexcept
{
  ++ptr_;
  return *this;
}

template< class T >
vasyakin::VectorIterator< T > vasyakin::VectorIterator< T >::operator++(int) noexcept
{
  VectorIterator< T > tmp = *this;
  ++ptr_;
  return tmp;
}

template< class T >
vasyakin::VectorIterator< T >& vasyakin::VectorIterator< T >::operator--() noexcept
{
  --ptr_;
  return *this;
}

template< class T >
vasyakin::VectorIterator< T > vasyakin::VectorIterator< T >::operator--(int) noexcept
{
  VectorIterator< T > tmp = *this;
  --ptr_;
  return tmp;
}

template < class T >
vasyakin::VectorIterator< T >& vasyakin::VectorIterator< T >::operator+=(std::ptrdiff_t n) noexcept
{
  ptr_ += n;
  return *this;
}

template< class T >
vasyakin::VectorIterator< T >& vasyakin::VectorIterator< T >::operator-=(std::ptrdiff_t n) noexcept
{
  ptr_ -= n;
  return *this;
}

template< class T >
vasyakin::VectorIterator< T > vasyakin::VectorIterator< T >::operator+(std::ptrdiff_t n) const noexcept
{
  return VectorIterator(ptr_ + n);
}

template< class T >
vasyakin::VectorIterator< T > vasyakin::VectorIterator< T >::operator-(std::ptrdiff_t n) const noexcept
{
  return VectorIterator(ptr_ - n);
}

template< class T >
std::ptrdiff_t vasyakin::VectorIterator< T >::operator-(const VectorIterator< T >& other) const noexcept
{
  return ptr_ - other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator==(const VectorIterator& other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator!=(const VectorIterator& other) const noexcept
{
  return ptr_ != other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator<(const VectorIterator& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator>(const VectorIterator& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator<=(const VectorIterator& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool vasyakin::VectorIterator< T >::operator>=(const VectorIterator& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif

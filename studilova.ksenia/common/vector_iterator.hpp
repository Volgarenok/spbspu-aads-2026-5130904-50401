#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <cstddef>

namespace studilova
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
studilova::VectorIterator< T >::VectorIterator(T* ptr) noexcept:
  ptr_(ptr)
{}

template< class T >
template< class U >
studilova::VectorIterator< T >::VectorIterator(const VectorIterator< U >& other) noexcept:
  ptr_(other.ptr_)
{}

template< class T >
T& studilova::VectorIterator< T >::operator*() const noexcept
{
  return *ptr_;
}

template< class T >
T* studilova::VectorIterator< T >::operator->() const noexcept
{
  return ptr_;
}

template< class T >
T& studilova::VectorIterator< T >::operator[](std::ptrdiff_t i) const noexcept
{
  return (ptr_[i]);
}

template< class T >
studilova::VectorIterator< T >& studilova::VectorIterator< T >::operator++() noexcept
{
  ++ptr_;
  return *this;
}

template< class T >
studilova::VectorIterator< T > studilova::VectorIterator< T >::operator++(int) noexcept
{
  VectorIterator< T > tmp = *this;
  ++ptr_;
  return tmp;
}

template< class T >
studilova::VectorIterator< T >& studilova::VectorIterator< T >::operator--() noexcept
{
  --ptr_;
  return *this;
}

template< class T >
studilova::VectorIterator< T > studilova::VectorIterator< T >::operator--(int) noexcept
{
  VectorIterator< T > tmp = *this;
  --ptr_;
  return tmp;
}

template < class T >
studilova::VectorIterator< T >& studilova::VectorIterator< T >::operator+=(std::ptrdiff_t n) noexcept
{
  ptr_ += n;
  return *this;
}

template< class T >
studilova::VectorIterator< T >& studilova::VectorIterator< T >::operator-=(std::ptrdiff_t n) noexcept
{
  ptr_ -= n;
  return *this;
}

template< class T >
studilova::VectorIterator< T > studilova::VectorIterator< T >::operator+(std::ptrdiff_t n) const noexcept
{
  return VectorIterator< T >(ptr_ + n);
}

template< class T >
studilova::VectorIterator< T > studilova::VectorIterator< T >::operator-(std::ptrdiff_t n) const noexcept
{
  return VectorIterator< T >(ptr_ - n);
}

template< class T >
std::ptrdiff_t studilova::VectorIterator< T >::operator-(const VectorIterator< T >& other) const noexcept
{
  return ptr_ - other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator==(const VectorIterator& other) const noexcept
{
  return ptr_ == other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator!=(const VectorIterator& other) const noexcept
{
  return ptr_ != other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator<(const VectorIterator& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator>(const VectorIterator& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator<=(const VectorIterator& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool studilova::VectorIterator< T >::operator>=(const VectorIterator& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif

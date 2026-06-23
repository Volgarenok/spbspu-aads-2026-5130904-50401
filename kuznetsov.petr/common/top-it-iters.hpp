#ifndef TOP_IT_ITERS_HPP
#define TOP_IT_ITERS_HPP
#include <cstddef>
namespace kuznetsov {

  template< class T >
  struct CIter {
    explicit CIter(T* ptr):
      ptr_(ptr)
    {}
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    CIter& operator++() noexcept;
    CIter operator++(int) noexcept;
    CIter& operator--() noexcept;
    CIter operator--(int) noexcept;
    CIter& operator+=(size_t n) noexcept;
    CIter& operator-=(size_t n) noexcept;
    CIter operator+(size_t n) const noexcept;
    CIter operator-(size_t n) const noexcept;
    std::ptrdiff_t operator-(const CIter&) const noexcept;
    bool operator==(const CIter&) const noexcept;
    bool operator!=(const CIter&) const noexcept;
    bool operator>(const CIter&) const noexcept;
    bool operator>=(const CIter&) const noexcept;
    bool operator<(const CIter&) const noexcept;
    bool operator<=(const CIter&) const noexcept;
  private:
    T* ptr_;
  };

  template< class T >
  struct Iter {
    explicit Iter(T* ptr):
      ptr_(ptr)
    {}
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    Iter& operator++() noexcept;
    Iter operator++(int) noexcept;
    Iter& operator--() noexcept;
    Iter operator--(int) noexcept;
    Iter& operator+=(size_t n) noexcept;
    Iter& operator-=(size_t n) noexcept;
    Iter operator+(size_t n) const noexcept;
    Iter operator-(size_t n) const noexcept;
    std::ptrdiff_t operator-(const Iter&) const noexcept;
    bool operator==(const Iter&) const noexcept;
    bool operator!=(const Iter&) const noexcept;
    bool operator>(const Iter&) const noexcept;
    bool operator>=(const Iter&) const noexcept;
    bool operator<(const Iter&) const noexcept;
    bool operator<=(const Iter&) const noexcept;
  private:
    T* ptr_;
  };
}

template< class T >
const T& kuznetsov::CIter< T >::operator*() const noexcept
{
  return *ptr_;
}

template< class T >
kuznetsov::CIter< T >& kuznetsov::CIter< T >::operator++() noexcept
{
  ++ptr_;
  return *this;
}

template< class T >
kuznetsov::CIter< T >& kuznetsov::CIter< T >::operator--() noexcept
{
  --ptr_;
  return *this;
}

template< class T >
kuznetsov::CIter< T >& kuznetsov::CIter< T >::operator+=(size_t n) noexcept
{
  ptr_ += n;
  return *this;
}

template< class T >
kuznetsov::CIter< T >& kuznetsov::CIter< T >::operator-=(size_t n) noexcept
{
  ptr_ -= n;
  return *this;
}

template< class T >
kuznetsov::CIter< T > kuznetsov::CIter< T >::operator+(size_t n) const noexcept
{
  return CIter< T >{ptr_ + n};
}

template< class T >
kuznetsov::CIter< T > kuznetsov::CIter< T >::operator-(size_t n) const noexcept
{
  return CIter< T >{ptr_ - n};
}

template< class T >
std::ptrdiff_t kuznetsov::CIter< T >::operator-(const CIter& rhs) const noexcept
{
  return this->ptr_ - rhs.ptr_;
}

template< class T >
bool kuznetsov::CIter< T >::operator==(const CIter& rhs) const noexcept
{
  return this->ptr_ == rhs.ptr_;
}

template< class T >
bool kuznetsov::CIter< T >::operator!=(const CIter& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
bool kuznetsov::CIter< T >::operator>(const CIter& rhs) const noexcept
{
  return this->ptr_ > rhs.ptr_;
}

template< class T >
bool kuznetsov::CIter< T >::operator>=(const CIter& rhs) const noexcept
{
  return this->ptr_ >= rhs.ptr_;
}

template< class T >
bool kuznetsov::CIter< T >::operator<(const CIter& rhs) const noexcept
{
  return this->ptr_ < rhs.ptr_;
}

template< class T >
bool kuznetsov::CIter< T >::operator<=(const CIter& rhs) const noexcept
{
  return this->ptr_ <= rhs.ptr_;
}

template< class T >
kuznetsov::CIter< T > kuznetsov::CIter< T >::operator++(int) noexcept
{
  CIter iter{ptr_};
  ++ptr_;
  return iter;
}

template< class T >
kuznetsov::CIter< T > kuznetsov::CIter< T >::operator--(int) noexcept
{
  CIter iter{ptr_};
  --ptr_;
  return iter;
}

template< class T >
T& kuznetsov::Iter< T >::operator*() const noexcept
{
  return *ptr_;
}

template< class T >
kuznetsov::Iter< T >& kuznetsov::Iter< T >::operator++() noexcept
{
  ++ptr_;
  return *this;
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Iter< T >::operator++(int) noexcept
{
  Iter it{ptr_};
  ++ptr_;
  return it;
}

template< class T >
kuznetsov::Iter< T >& kuznetsov::Iter< T >::operator--() noexcept
{
  --ptr_;
  return *this;
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Iter< T >::operator--(int) noexcept
{
  Iter it{ptr_};
  --ptr_;
  return it;
}

template< class T >
kuznetsov::Iter< T >& kuznetsov::Iter< T >::operator+=(size_t n) noexcept
{
  ptr_ += n;
  return *this;
}

template< class T >
kuznetsov::Iter< T >& kuznetsov::Iter< T >::operator-=(size_t n) noexcept
{
  ptr_ -= n;
  return *this;
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Iter< T >::operator+(size_t n) const noexcept
{
  return Iter{ptr_ + n};
}

template< class T >
kuznetsov::Iter< T > kuznetsov::Iter< T >::operator-(size_t n) const noexcept
{
  return Iter{ptr_ - n};
}

template< class T >
std::ptrdiff_t kuznetsov::Iter< T >::operator-(const Iter& rhs) const noexcept
{
  return ptr_ - rhs.ptr_;
}

template< class T >
bool kuznetsov::Iter< T >::operator==(const Iter& rhs) const noexcept
{
  return ptr_ == rhs.ptr_;
}

template< class T >
bool kuznetsov::Iter< T >::operator!=(const Iter& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
bool kuznetsov::Iter< T >::operator>(const Iter& rhs) const noexcept
{
  return ptr_ > rhs.ptr_;
}

template< class T >
bool kuznetsov::Iter< T >::operator>=(const Iter& rhs) const noexcept
{
  return ptr_ >= rhs.ptr_;
}

template< class T >
bool kuznetsov::Iter< T >::operator<(const Iter& rhs) const noexcept
{
  return ptr_ < rhs.ptr_;
}

template< class T >
bool kuznetsov::Iter< T >::operator<=(const Iter& rhs) const noexcept
{
  return ptr_ <= rhs.ptr_;
}

template< class T >
const T* kuznetsov::CIter< T >::operator->() const noexcept
{
  return ptr_;
}

template< class T >
T* kuznetsov::Iter< T >::operator->() const noexcept
{
  return ptr_;
}

#endif


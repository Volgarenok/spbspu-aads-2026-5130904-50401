#ifndef TOP_IT_CONST_ITERATOR_HPP
#define TOP_IT_CONST_ITERATOR_HPP
#include <cassert>
#include <cstddef>
namespace topit
{
  template< class T >
  struct VecConstIter {
  public:
    VecConstIter();
    VecConstIter(const T* ptr);
    VecConstIter< T >& operator++();
    VecConstIter< T > operator++(int);
    VecConstIter< T > operator+(size_t) const noexcept;


    VecConstIter< T >& operator--();
    VecConstIter< T > operator--(int);
    VecConstIter< T > operator-(size_t) const noexcept;

    std::ptrdiff_t operator-(const VecConstIter& other) const;

    bool operator==(const VecConstIter& other) const noexcept;
    bool operator!=(const VecConstIter& other) const noexcept;
    bool operator<(const VecConstIter& other) const noexcept;
    bool operator>(const VecConstIter& other) const noexcept;
    bool operator<=(const VecConstIter& other) const noexcept;
    bool operator>=(const VecConstIter& other) const noexcept;

    const T& operator*() const;
    const T* operator->() const;
  private:
    const T* ptr_;
  };
}

template< class T >
topit::VecConstIter< T >::VecConstIter():
  ptr_(nullptr)
{}


template< class T >
topit::VecConstIter< T >::VecConstIter(const T* ptr):
  ptr_(ptr)
{}

template< class T >
topit::VecConstIter< T >& topit::VecConstIter< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
topit::VecConstIter< T > topit::VecConstIter< T >::operator++(int)
{
  VecConstIter< T >tmp{*this};
  ++(*this);
  return tmp;
}

template< class T >
topit::VecConstIter< T > topit::VecConstIter< T >::operator+(size_t n) const noexcept
{
  return {ptr_ + n};
}


template< class T >
topit::VecConstIter< T >& topit::VecConstIter< T >::operator--()
{
  ptr_--;
  return *this;
}

template< class T >
topit::VecConstIter< T > topit::VecConstIter< T >::operator--(int)
{
  VecConstIter< T >tmp{*this};
  --(*this);
  return tmp;
}

template< class T >
topit::VecConstIter< T > topit::VecConstIter< T >::operator-(size_t n) const noexcept
{
  return {ptr_ - n};
}

template< class T >
std::ptrdiff_t topit::VecConstIter< T >::operator-(const VecConstIter< T >& other) const
{
  return ptr_ - other.ptr_;
}


template< class T >
const T& topit::VecConstIter< T >::operator*() const
{
  return *ptr_;
}

template< class T >
const T* topit::VecConstIter< T >::operator->() const
{
  return std::addressof(ptr_);
}

template< class T >
bool topit::VecConstIter< T >::operator!=(const VecConstIter< T >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
bool topit::VecConstIter< T >::operator==(const VecConstIter< T >& rhs) const noexcept
{
  return ptr_ == rhs.ptr_;
}

template< class T >
bool topit::VecConstIter< T >::operator<(const VecConstIter& other) const noexcept
{
  return ptr_ < other.ptr_;
}

template< class T >
bool topit::VecConstIter< T >::operator>(const VecConstIter& other) const noexcept
{
  return ptr_ > other.ptr_;
}

template< class T >
bool topit::VecConstIter< T >::operator<=(const VecConstIter& other) const noexcept
{
  return ptr_ <= other.ptr_;
}

template< class T >
bool topit::VecConstIter< T >::operator>=(const VecConstIter& other) const noexcept
{
  return ptr_ >= other.ptr_;
}

#endif

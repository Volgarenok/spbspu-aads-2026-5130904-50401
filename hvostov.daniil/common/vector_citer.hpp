#ifndef TOP_IT_CONST_ITERATOR_HPP
#define TOP_IT_CONST_ITERATOR_HPP
#include <cstddef>

namespace hvostov {
  template< class T >
  class Vector;

  template< class T >
  class CIterator {
  public:
    CIterator();
    CIterator(T* ptr);
    CIterator(const CIterator< T >&);
    CIterator(CIterator< T >&&) noexcept;
    CIterator< T >& operator=(const CIterator< T >&);
    CIterator< T >& operator=(CIterator< T >&&) noexcept;

    const T& operator*() const;
    const T* operator->() const;
    const T& operator[](std::ptrdiff_t n) const;
    CIterator< T >& operator++();
    CIterator< T > operator++(int);
    CIterator< T >& operator--();
    CIterator< T > operator--(int);
    CIterator< T > operator+(std::ptrdiff_t n) const;
    CIterator< T > operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const CIterator< T >&) const;

    bool operator==(const CIterator< T >&) const noexcept;
    bool operator!=(const CIterator< T >&) const noexcept;
    bool operator<(const CIterator< T >&) const noexcept;
    bool operator>(const CIterator< T >&) const noexcept;
    bool operator<=(const CIterator< T >&) const noexcept;
    bool operator>=(const CIterator< T >&) const noexcept;

  private:
    friend class Vector< T >;
    const T* ptr_;
  };
}

template< class T >
hvostov::CIterator< T >::CIterator():
  ptr_(nullptr)
{}

template< class T >
hvostov::CIterator< T >::CIterator(T* ptr):
  ptr_(ptr)
{}

template< class T >
hvostov::CIterator< T >::CIterator(const CIterator< T >& it):
  ptr_(it.ptr_)
{}

template< class T >
hvostov::CIterator< T >::CIterator(CIterator< T >&& it) noexcept:
  ptr_(it.ptr_)
{
  it.ptr_ = nullptr;
}

template< class T >
hvostov::CIterator< T >& hvostov::CIterator< T >::operator=(const CIterator< T >& it)
{
  if (this != &it) {
    ptr_ = it.ptr_;
  }
  return *this;
}

template< class T >
hvostov::CIterator< T >& hvostov::CIterator< T >::operator=(CIterator< T >&& it) noexcept
{
  if (this != &it) {
    ptr_ = it.ptr_;
    it.ptr_ = nullptr;
  }
  return *this;
}

template< class T >
const T& hvostov::CIterator< T >::operator*() const
{
  return *ptr_;
}

template< class T >
const T* hvostov::CIterator< T >::operator->() const
{
  return ptr_;
}

template< class T >
const T& hvostov::CIterator< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
hvostov::CIterator< T >& hvostov::CIterator< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
hvostov::CIterator< T > hvostov::CIterator< T >::operator++(int)
{
  CIterator< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
hvostov::CIterator< T >& hvostov::CIterator< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
hvostov::CIterator< T > hvostov::CIterator< T >::operator--(int)
{
  CIterator< T > temp = *this;
  --(*this);
  return temp;
}

template< class T >
hvostov::CIterator< T > hvostov::CIterator< T >::operator+(std::ptrdiff_t n) const
{
  return CIterator< T >{ptr_ + n};
}

template< class T >
hvostov::CIterator< T > hvostov::CIterator< T >::operator-(std::ptrdiff_t n) const
{
  return CIterator< T >{ptr_ - n};
}

template< class T >
std::ptrdiff_t hvostov::CIterator< T >::operator-(const CIterator< T >& it) const
{
  return ptr_ - it.ptr_;
}

template< class T >
bool hvostov::CIterator< T >::operator==(const CIterator< T >& it) const noexcept
{
  return ptr_ == it.ptr_;
}

template< class T >
bool hvostov::CIterator< T >::operator!=(const CIterator< T >& it) const noexcept
{
  return !(*this == it);
}

template< class T >
bool hvostov::CIterator< T >::operator<(const CIterator< T >& it) const noexcept
{
  return ptr_ < it.ptr_;
}

template< class T >
bool hvostov::CIterator< T >::operator>(const CIterator< T >& it) const noexcept
{
  return ptr_ > it.ptr_;
}

template< class T >
bool hvostov::CIterator< T >::operator<=(const CIterator< T >& it) const noexcept
{
  return ptr_ <= it.ptr_;
}

template< class T >
bool hvostov::CIterator< T >::operator>=(const CIterator< T >& it) const noexcept
{
  return ptr_ >= it.ptr_;
}

#endif

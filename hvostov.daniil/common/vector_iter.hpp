#ifndef TOP_IT_ITERATOR_HPP
#define TOP_IT_ITERATOR_HPP
#include <cstddef>

namespace hvostov {
  template< class T >
  class Vector;

  template< class T >
  class Iterator {
  public:
    Iterator();
    Iterator(T* ptr);
    Iterator(const Iterator< T >&);
    Iterator(Iterator< T >&&) noexcept;
    Iterator< T >& operator=(const Iterator< T >&);
    Iterator< T >& operator=(Iterator< T >&&) noexcept;

    T& operator*() const;
    T* operator->() const;
    T& operator[](std::ptrdiff_t n) const;
    Iterator< T >& operator++();
    Iterator< T > operator++(int);
    Iterator< T >& operator--();
    Iterator< T > operator--(int);
    Iterator< T > operator+(std::ptrdiff_t n) const;
    Iterator< T > operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const Iterator< T >&) const;

    bool operator==(const Iterator< T >&) const noexcept;
    bool operator!=(const Iterator< T >&) const noexcept;
    bool operator<(const Iterator< T >&) const noexcept;
    bool operator>(const Iterator< T >&) const noexcept;
    bool operator<=(const Iterator< T >&) const noexcept;
    bool operator>=(const Iterator< T >&) const noexcept;

  private:
    friend class Vector< T >;
    T* ptr_;
  };
}

template< class T >
hvostov::Iterator< T >::Iterator():
  ptr_(nullptr)
{}

template< class T >
hvostov::Iterator< T >::Iterator(T* ptr):
  ptr_(ptr)
{}

template< class T >
hvostov::Iterator< T >::Iterator(const Iterator< T >& it):
  ptr_(it.ptr_)
{}

template< class T >
hvostov::Iterator< T >::Iterator(Iterator< T >&& it) noexcept:
  ptr_(it.ptr_)
{
  it.ptr_ = nullptr;
}

template< class T >
hvostov::Iterator< T >& hvostov::Iterator< T >::operator=(const Iterator< T >& it)
{
  if (this != &it) {
    ptr_ = it.ptr_;
  }
  return *this;
}

template< class T >
hvostov::Iterator< T >& hvostov::Iterator< T >::operator=(Iterator< T >&& it) noexcept
{
  if (this != &it) {
    ptr_ = it.ptr_;
    it.ptr_ = nullptr;
  }
  return *this;
}

template< class T >
T& hvostov::Iterator< T >::operator*() const
{
  return *ptr_;
}

template< class T >
T* hvostov::Iterator< T >::operator->() const
{
  return ptr_;
}

template< class T >
T& hvostov::Iterator< T >::operator[](std::ptrdiff_t n) const
{
  return ptr_[n];
}

template< class T >
hvostov::Iterator< T >& hvostov::Iterator< T >::operator++()
{
  ++ptr_;
  return *this;
}

template< class T >
hvostov::Iterator< T > hvostov::Iterator< T >::operator++(int)
{
  Iterator< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
hvostov::Iterator< T >& hvostov::Iterator< T >::operator--()
{
  --ptr_;
  return *this;
}

template< class T >
hvostov::Iterator< T > hvostov::Iterator< T >::operator--(int)
{
  Iterator< T > temp = *this;
  --(*this);
  return temp;
}

template< class T >
hvostov::Iterator< T > hvostov::Iterator< T >::operator+(std::ptrdiff_t n) const
{
  return Iterator< T >{ptr_ + n};
}

template< class T >
hvostov::Iterator< T > hvostov::Iterator< T >::operator-(std::ptrdiff_t n) const
{
  return Iterator< T >{ptr_ - n};
}

template< class T >
std::ptrdiff_t hvostov::Iterator< T >::operator-(const Iterator< T >& it) const
{
  return ptr_ - it.ptr_;
}

template< class T >
bool hvostov::Iterator< T >::operator==(const Iterator< T >& it) const noexcept
{
  return ptr_ == it.ptr_;
}

template< class T >
bool hvostov::Iterator< T >::operator!=(const Iterator< T >& it) const noexcept
{
  return !(*this == it);
}

template< class T >
bool hvostov::Iterator< T >::operator<(const Iterator< T >& it) const noexcept
{
  return ptr_ < it.ptr_;
}

template< class T >
bool hvostov::Iterator< T >::operator>(const Iterator< T >& it) const noexcept
{
  return ptr_ > it.ptr_;
}

template< class T >
bool hvostov::Iterator< T >::operator<=(const Iterator< T >& it) const noexcept
{
  return ptr_ <= it.ptr_;
}

template< class T >
bool hvostov::Iterator< T >::operator>=(const Iterator< T >& it) const noexcept
{
  return ptr_ >= it.ptr_;
}

#endif
